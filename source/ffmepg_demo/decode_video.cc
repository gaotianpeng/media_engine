#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtest/gtest.h"
extern "C" {
#include "libavutil/frame.h"
#include "libavutil/mem.h"
#include "libavcodec/avcodec.h"
}

#define VIDEO_INBUF_SIZE 20480
#define VIDEO_REFILL_THRESH 4096

/*
* ffmpeg -i in.mp4 -vcodec libx264 -an -f h264 out.h264
* ffmpeg -i in.mp4 -vcodec mpeg2video -an -f mpeg2video out.mpeg2
* ffplay -pixel_format yuv420p -video_size 1280x720 -framerate 25 out.yuv
*/
static char err_buf[128] = { 0 };
static char* av_get_err(int errnum) {
	av_strerror(errnum, err_buf, 128);
	return err_buf;
}

static void decode(AVCodecContext* dec_ctx, AVPacket* pkt, AVFrame* frame,
		FILE* out_file) {
	int ret = -1;
	ret = avcodec_send_packet(dec_ctx, pkt);
	if (ret == AVERROR(EAGAIN)) {
		fprintf(stderr, "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
	}
	else if (ret < 0) {
		fprintf(stderr, "Error submitting the packet to the decoder, err:%s, pkt_size:%d\n",
			av_get_err(ret), pkt->size);
		return;
	}

	while (ret >= 0) {
		ret = avcodec_receive_frame(dec_ctx, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			return;
		}
		else if (ret < 0) {
			fprintf(stderr, "Error during decoding\n");
			exit(1);
		}

		for (int j = 0; j < frame->height; j++)
			fwrite(frame->data[0] + j * frame->linesize[0], 1, frame->width, out_file);
		for (int j = 0; j < frame->height / 2; j++)
			fwrite(frame->data[1] + j * frame->linesize[1], 1, frame->width / 2, out_file);
		for (int j = 0; j < frame->height / 2; j++)
			fwrite(frame->data[2] + j * frame->linesize[2], 1, frame->width / 2, out_file);
	}
}

#ifdef DECODE_VIDEO

TEST(FFmpegDecodeVideo, DecodeVideo) {
	const char* in_file = "D:/av_data/out1280x720.h264";
	const char* out_file = "D:/av_data/out1280x720.yuv";;
	const AVCodec* codec = NULL;
	AVCodecContext* codec_ctx = NULL;
	AVCodecParserContext* parser = NULL;
	int len = 0;
	int ret = -1;
	FILE* in_fp = NULL;
	FILE* out_fp = NULL;

	uint8_t in_buf[VIDEO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
	uint8_t* data = NULL;
	AVPacket* pkt = NULL;
	AVFrame* decoded_frame = NULL;

	pkt = av_packet_alloc();
	enum AVCodecID video_codec_id = AV_CODEC_ID_NONE;
	if (strstr(in_file, "264") != NULL) {
		video_codec_id = AV_CODEC_ID_H264;
	}
	else if (strstr(in_file, "mpeg2") != NULL) {
		video_codec_id = AV_CODEC_ID_MPEG2VIDEO;
	}
	else {
		ASSERT_TRUE(true);
	}

	codec = avcodec_find_decoder(video_codec_id);
	ASSERT_TRUE(codec != NULL);

	parser = av_parser_init(codec->id);
	ASSERT_TRUE(parser != NULL);

	codec_ctx = avcodec_alloc_context3(codec);
	ASSERT_TRUE(codec_ctx != NULL);

	ret = avcodec_open2(codec_ctx, codec, NULL);
	ASSERT_TRUE(ret >= 0);

	in_fp = fopen(in_file, "rb");
	ASSERT_TRUE(in_fp != NULL);

	out_fp = fopen(out_file, "wb");
	ASSERT_TRUE(out_fp != NULL);

	data = in_buf;
	int data_size = fread(in_buf, 1, VIDEO_INBUF_SIZE, in_fp);
	while (data_size > 0) {
		if (!decoded_frame) {
			decoded_frame = av_frame_alloc();
			ASSERT_TRUE(decoded_frame != NULL);
		}
		ret = av_parser_parse2(parser, codec_ctx, &pkt->data, &pkt->size,
				data, data_size,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
		
		ASSERT_TRUE(ret >= 0);
		data += ret;
		data_size -= ret;

		if (pkt->size) {
			decode(codec_ctx, pkt, decoded_frame, out_fp);
		}

		if (data_size < VIDEO_REFILL_THRESH) {
			memmove(in_buf, data, data_size);
			data = in_buf;
			len = fread(data + data_size, 1, VIDEO_INBUF_SIZE - data_size, in_fp);
			if (len > 0) {
				data_size += len;
			}
		}
	}

	pkt->data = NULL;
	pkt->data = 0;
	decode(codec_ctx, pkt, decoded_frame, out_fp);

	fclose(out_fp);
	fclose(in_fp);

	avcodec_free_context(&codec_ctx);
}

#endif