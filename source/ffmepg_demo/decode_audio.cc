#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtest/gtest.h>
#include <iostream>

extern "C" {
#include "libavutil/frame.h"
#include "libavutil/mem.h"
#include "libavcodec/avcodec.h"
}

using namespace std;

#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

static char err_buf[128] = {0};

static char* av_get_err(int errnum) {
    av_strerror(errnum, err_buf, 128);
    return err_buf;
}

static void print_sample_format(const AVFrame* frame) {
    printf("audio sample rate: %uHz\n", frame->sample_rate);
    printf("audio channel: %u\n", frame->channels);
    // AVSampleFormat
    printf("AVSampleFormat: %u\n",
         frame->format);  // 格式需要注意，实际存储到本地文件时已经改成交错模式
}

static void decode(AVCodecContext* dec_ctx, AVPacket* pkt, AVFrame* frame,
                   FILE* out_fp) {
    int i, ch;
    int ret, data_size;
    // send the packet with the compresed data to the decoder
    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret == AVERROR(EAGAIN)) {
        fprintf(stderr,
              "Receive_frame and send_packet both returned EAGAIN, which is an "
              "API violation.\n");
    } else if (ret < 0) {
        fprintf(
          stderr,
          "Error submitting the packet to the decoder, err:%s, pkt_size:%d\n",
          av_get_err(ret), pkt->size);
        return;
    }

    // read all the output frames
    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }
        data_size = av_get_bytes_per_sample(dec_ctx->sample_fmt);
        if (data_size < 0) {
            fprintf(stderr, "Failed to calculate data size\n");
            exit(1);
        }
        static int s_print_format = 0;
        if (s_print_format == 0) {
            s_print_format = 1;
            print_sample_format(frame);
        }
        /**
            P表示Planar（平面），其数据格式排列方式为 :
            LLLLLLRRRRRRLLLLLLRRRRRRLLLLLLRRRRRRL...（每个LLLLLLRRRRRR为一个音频帧）
            而不带P的数据格式（即交错排列）排列方式为：
            LRLRLRLRLRLRLRLRLRLRLRLRLRLRLRLRLRLRL...（每个LR为一个音频样本）
         播放范例：   ffplay -ar 44100 -ac 2 -f f32le 720_ue4.pcm
         */
        for (i = 0; i < frame->nb_samples; i++) {
            for (ch = 0; ch < dec_ctx->channels;
                    ch++)  // 交错的方式写入, 大部分float的格式输出
            fwrite(frame->data[ch] + data_size * i, 1, data_size, out_fp);
        }
    }
}

TEST(FFmpegDecodeAudio, DecodeAudio) { 
	const char* in_file = "d:/av_data/720_ue4.aac";
    const char* out_file = "d:/av_data/720_ue4.pcm";
    AVCodecContext* a_codec_ctx = NULL;
    enum AVCodecID a_codec_id = AV_CODEC_ID_NONE;
	AVPacket* pkt = NULL;
    AVCodec* a_codec = NULL;
    AVCodecParserContext* parser = NULL;
    FILE* in_fp = NULL;
    FILE* out_fp = NULL;
    AVFrame* decoded_frame = NULL;

	pkt = av_packet_alloc();
	EXPECT_TRUE(pkt != NULL);
	

    if (strstr(in_file, "aac") != NULL) {
        a_codec_id = AV_CODEC_ID_AAC;
	} else if (strstr(in_file, "mp3") != NULL) {
         a_codec_id = AV_CODEC_ID_MP3;
    } else {
         goto exit_pos;
	}

	// 查找解码器
    a_codec = avcodec_find_decoder(a_codec_id);
    ASSERT_TRUE(a_codec != NULL);


	parser = av_parser_init(a_codec->id);
    ASSERT_TRUE(parser != NULL);

	// 分配 codec 上下文
	a_codec_ctx = avcodec_alloc_context3(a_codec);
    ASSERT_TRUE(a_codec_ctx != NULL);

    // 打开编码器
    ASSERT_TRUE(avcodec_open2(a_codec_ctx, a_codec, NULL) >= 0);
	
    in_fp = fopen(in_file, "rb");
    ASSERT_TRUE(in_file != NULL);
    out_fp = fopen(out_file, "wb");
    ASSERT_TRUE(out_fp != NULL);

    uint8_t inbuf[AUDIO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t* data = NULL;
    data = inbuf;
    int data_size = fread(inbuf, 1, AUDIO_INBUF_SIZE, in_fp);
    int ret = -1;
    while (data_size > 0) {
        if (!decoded_frame) {
            decoded_frame = av_frame_alloc();
            ASSERT_TRUE(decoded_frame != NULL);
        }

        ret = av_parser_parse2(parser, a_codec_ctx, &pkt->data, &pkt->size, data,
                             data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        ASSERT_TRUE(ret >= 0);

        data += ret;  // 跳过已解析的数据
        data_size -= ret;

        if (pkt->size) {
            decode(a_codec_ctx, pkt, decoded_frame, out_fp);
        }

        // 如果数据不够，则再次读取
        if (data_size < AUDIO_REFILL_THRESH) {
            // 将上次解码后剩余的数据copy至buffer的起始位置
            memmove(inbuf, data, data_size);
            data = inbuf;
            // 读取数据长度 = AUDIO_INBUF_SIZE - data_size
            int len =
                fread(data + data_size, 1, AUDIO_INBUF_SIZE - data_size, in_fp);
            if (len > 0) {
                data_size += len;
            }
        }
    }

    // 冲刷编码器
    pkt->data = NULL;
    pkt->size = 0;
    decode(a_codec_ctx, pkt, decoded_frame, out_fp);

exit_pos:
    avcodec_free_context(&a_codec_ctx);
    av_frame_free(&decoded_frame);
    av_parser_close(parser);
	av_packet_free(&pkt);
    fclose(in_fp);
    fclose(out_fp);
}

