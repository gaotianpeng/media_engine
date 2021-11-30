#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/frame.h"
#include "libavutil/samplefmt.h"
#include "libavutil/opt.h"
}

#include "gtest/gtest.h"

//#define AUDIO_ENCODE 1

static bool check_sample_fmt(const AVCodec* codec, enum AVSampleFormat sample_fmt) {
	const enum AVSampleFormat* p = codec->sample_fmts;

	while (*p != AV_SAMPLE_FMT_NONE) {
		if (*p == sample_fmt)
			return true;
		p++;
	}

	return false;
}

static bool check_sample_rate(const AVCodec* codec, const int sample_rate) {
	const int* p = codec->supported_samplerates;
	while (*p != 0) {
		printf("%s support %dhz\n", codec->name, *p);
		if (*p == sample_rate)
			return true;
		p++;
	}
	return false;
}

static bool check_channel_layout(const AVCodec* codec, const uint64_t channel_layout) {
	const uint64_t* p = codec->channel_layouts;
	if (!p) {
		printf("the codec %s no set channel_layouts\n", codec->name);
		return true;
	}
	while (*p != 0) {
		printf("%s support channel_layout %d\n", codec->name, *p);
		if (*p == channel_layout)
			return true;
		p++;
	}
	return false;
}

static void get_adts_header(AVCodecContext* ctx, uint8_t* adts_header, int aac_length) {
	uint8_t freq_idx = 0;    //0: 96000 Hz  3: 48000 Hz 4: 44100 Hz
	switch (ctx->sample_rate) {
	case 96000: freq_idx = 0; break;
	case 88200: freq_idx = 1; break;
	case 64000: freq_idx = 2; break;
	case 48000: freq_idx = 3; break;
	case 44100: freq_idx = 4; break;
	case 32000: freq_idx = 5; break;
	case 24000: freq_idx = 6; break;
	case 22050: freq_idx = 7; break;
	case 16000: freq_idx = 8; break;
	case 12000: freq_idx = 9; break;
	case 11025: freq_idx = 10; break;
	case 8000: freq_idx = 11; break;
	case 7350: freq_idx = 12; break;
	default: freq_idx = 4; break;
	}
	uint8_t chanCfg = ctx->channels;
	uint32_t frame_length = aac_length + 7;
	adts_header[0] = 0xFF;
	adts_header[1] = 0xF1;
	adts_header[2] = ((ctx->profile) << 6) + (freq_idx << 2) + (chanCfg >> 2);
	adts_header[3] = (((chanCfg & 3) << 6) + (frame_length >> 11));
	adts_header[4] = ((frame_length & 0x7FF) >> 3);
	adts_header[5] = (((frame_length & 7) << 5) + 0x1F);
	adts_header[6] = 0xFC;
}

void f32le_convert_to_fltp(float *f32le, float *fltp, int nb_samples) {
    float *fltp_l = fltp;
    float *fltp_r = fltp + nb_samples;
    for(int i = 0; i < nb_samples; i++) {
        fltp_l[i] = f32le[i*2];     // 0 1   - 2 3
        fltp_r[i] = f32le[i*2+1];
    }
}

static int encode(AVCodecContext* ctx, AVFrame* frame, AVPacket* pkt, FILE* output) {
	int ret;
	ret = avcodec_send_frame(ctx, frame);
	if (ret < 0) {
		fprintf(stderr, "Error sending the frame to the encoder\n");
		return -1;
	}

	while (ret >= 0) {
		ret = avcodec_receive_packet(ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			return 0;
		} else if (ret < 0) {
			fprintf(stderr, "Error encoding audio frame\n");
			return -1;
		}
	}

	int len = fwrite(pkt->data, 1, pkt->size, output);
	if (len != pkt->size) {
		fprintf(stderr, "fwrite aac data failed\n");
		return -1;
	}

	return -1;
}

#ifdef AUDIO_ENCODE

TEST(FFmpegAudioDecode, AudioDecode) {
	const char* in_pcm_file = "d:/av_data/720_ue4.pcm";
	const char* out_aac_file = "d:/av_data/720_ue4_no_adts.aac";
	FILE* in_fp = NULL;
	FILE* out_fp = NULL;

	const AVCodec* codec = NULL;
	AVCodecContext* codec_ctx = NULL;
	AVFrame* frame = NULL;
	AVPacket* pkt = NULL;

	int ret = 0;

	enum AVCodecID codec_id = AV_CODEC_ID_AAC;
	const char* codec_name = "aac";

	codec = avcodec_find_encoder_by_name(codec_name);
	ASSERT_TRUE(codec != NULL);

	codec_ctx = avcodec_alloc_context3(codec);
	ASSERT_TRUE(codec_ctx != NULL);

	codec_ctx->codec_id = codec_id;
	codec_ctx->codec_type = AVMEDIA_TYPE_AUDIO;
	codec_ctx->bit_rate = 128 * 1024;
	codec_ctx->channel_layout = AV_CH_LAYOUT_STEREO;
	codec_ctx->sample_rate = 44100;
	codec_ctx->channels = av_get_channel_layout_nb_channels(codec_ctx->channel_layout);
	codec_ctx->profile = FF_PROFILE_AAC_LOW;
	codec_ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;

	ASSERT_TRUE(check_sample_fmt(codec, codec_ctx->sample_fmt));
	ASSERT_TRUE(check_sample_rate(codec, codec_ctx->sample_rate));
	ASSERT_TRUE(check_channel_layout(codec, codec_ctx->channel_layout));

	printf("\n\nAudio encode config\n");
	printf("bit_rate:%ldkbps\n", codec_ctx->bit_rate/1024);
	printf("sample_rate:%d\n", codec_ctx->sample_rate);
	printf("sample_fmt:%s\n", av_get_sample_fmt_name(codec_ctx->sample_fmt));
	printf("channels:%d\n", codec_ctx->channels);
	printf("1 frame_size:%d\n", codec_ctx->frame_size);
	/*
	*	the default aac of ffmpeg is not with adts
	*	fdk_aac default with adts
	*	here, we force to have no adts
	*/ 
	codec_ctx->flags = AV_CODEC_FLAG_GLOBAL_HEADER;
	ret = avcodec_open2(codec_ctx, codec, NULL);
	ASSERT_TRUE(ret >= 0);
	printf("2 frame_size:%d\n\n", codec_ctx->frame_size);

	in_fp = fopen(in_pcm_file, "rb");
	ASSERT_TRUE(in_fp != NULL);

	out_fp = fopen(out_aac_file, "wb");
	ASSERT_TRUE(out_fp != NULL);

	pkt = av_packet_alloc();
	ASSERT_TRUE(pkt != NULL);

	frame = av_frame_alloc();
	ASSERT_TRUE(frame != NULL);

	/*
		每次送多少数据给编码器由
		1) frame_size(每帧单个通道的采样点数)
		2) sample_fmt
		3) channel_layout
		三要素决定
	*/
	frame->nb_samples = codec_ctx->frame_size;
	frame->format = codec_ctx->sample_fmt;
	frame->channel_layout = codec_ctx->channel_layout;
	frame->channels = av_get_channel_layout_nb_channels(frame->channel_layout);
	printf("frame nb_sampels:%d\n", frame->nb_samples);
	printf("frame sample_fmt:%d\n", frame->format);
	printf("frame channel_layout:%d\n", frame->channel_layout);

	ret = av_frame_get_buffer(frame, 0);
	ASSERT_TRUE(ret >= 0);

	int frame_bytes = av_get_bytes_per_sample((AVSampleFormat)frame->format)*frame->channels*frame->nb_samples;
	printf("frame_bytes %d\n", frame_bytes);
	uint8_t* pcm_buf = (uint8_t*)malloc(frame_bytes);
	ASSERT_TRUE(pcm_buf != NULL);
    uint8_t *pcm_temp_buf = (uint8_t *)malloc(frame_bytes);
	ASSERT_TRUE(pcm_temp_buf != NULL);

	int64_t pts = 0;
	printf("start encode\n");
	for (;;) {
		memset(pcm_buf, 0, frame_bytes);
		int read_bytes = fread(pcm_buf, 1, frame_bytes, in_fp);
		if (read_bytes <= 0) {
			break;
		}

		ret = av_frame_make_writable(frame);
		if(ret != 0) {
            printf("av_frame_make_writable failed, ret = %d\n", ret);
		}

		if (AV_SAMPLE_FMT_S16 == frame->format) {
			ret = av_samples_fill_arrays(frame->data, frame->linesize,
					pcm_buf, frame->channels,
					frame->nb_samples, (AVSampleFormat)frame->format, 0);

		} else {
			memset(pcm_temp_buf, 0, frame_bytes);
			f32le_convert_to_fltp((float*)pcm_buf, (float*)pcm_temp_buf, frame->nb_samples);
			ret = av_samples_fill_arrays(frame->data, frame->linesize,
					pcm_temp_buf, frame->channels,
					frame->nb_samples, (AVSampleFormat)frame->format, 0);
		}

		pts += frame->nb_samples;
		frame->pts = pts;	// 使用采样率作为pts的单位, 具体换算成秒pts*1/采样率
		ret = encode(codec_ctx, frame, pkt, out_fp);
		if(ret < 0) {
            printf("encode failed\n");
            break;
        }
	}
    encode(codec_ctx, NULL, pkt, out_fp);
    fclose(in_fp);
    fclose(out_fp);
    if(pcm_buf) {
        free(pcm_buf);
    }
    if (pcm_temp_buf) {
        free(pcm_temp_buf);
    }
    av_frame_free(&frame);
    av_packet_free(&pkt);
    avcodec_free_context(&codec_ctx);
    printf("main finish, please enter Enter and exit\n");
}
#endif