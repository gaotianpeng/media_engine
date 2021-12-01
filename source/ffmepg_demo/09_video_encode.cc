#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/time.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
}

#include "gtest/gtest.h"

static int64_t get_time() {
	return av_gettime_relative() / 1000;
}

static int encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt, FILE* out_file) {
	int ret;
	if (frame) {
		printf("Send frame %lld\n", frame->pts);
	}

	ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0) {
		fprintf(stderr, "Error sending a frame for encoding\n");
		return -1;
	}

	while (ret >= 0) {
		ret = avcodec_receive_packet(enc_ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			return 0;
		}
		else if (ret < 0) {
			fprintf(stderr, "Error encoding audio frame\n");
			return -1;
		}

		if (pkt->flags & AV_PKT_FLAG_KEY)
			printf("Write packet flags:%d pts:%3lld" " dts:%3lld"" (size:%5d)\n",
				pkt->flags, pkt->pts, pkt->dts, pkt->size);
		if (!pkt->flags)
			printf("Write packet flags:%d pts:%3lld" " dts:%3lld" " (size:%5d)\n",
				pkt->flags, pkt->pts, pkt->dts, pkt->size);
		fwrite(pkt->data, 1, pkt->size, out_file);
	}
	return 0;
}

#ifdef VIDEO_ENCODE
TEST(FFmpegVideoEncode, VideoEncode) {
	char* in_yuv_file = NULL;
	char* out_h264_file = NULL;
	FILE* in_fp = NULL;
	FILE* out_fp = NULL;

	const char* codec_name = NULL;
	const AVCodec* codec = NULL;
	AVCodecContext* codec_ctx = NULL;
	AVFrame* frame = NULL;
	AVPacket* pkt = NULL;
	int ret = 0;

	in_yuv_file = "d:/av_data/yuv420p_1280x720.yuv";
	out_h264_file = "d:/av_data/yuv420p_1280x720.h264";
	codec_name = "libx264";

	codec = avcodec_find_encoder_by_name(codec_name);
	ASSERT_TRUE(codec != NULL);

	codec_ctx = avcodec_alloc_context3(codec);
	ASSERT_TRUE(codec_ctx != NULL);

	codec_ctx->width = 1280;
	codec_ctx->height = 720;

	codec_ctx->time_base.num = 1;
	codec_ctx->time_base.den = 25;

	codec_ctx->framerate.num = 25;
	codec_ctx->framerate.den = 1;
	/* 设置I帧间隔
	 * 如果frame->pict_type设置为AV_PICTURE_TYPE_I, 则忽略gop_size的设置，一直当做I帧进行编码
	 */
	codec_ctx->gop_size = 25;
	codec_ctx->max_b_frames = 2;
	codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

	if (codec->id == AV_CODEC_ID_H264) {
		ret = av_opt_set(codec_ctx->priv_data, "preset", "medium", 0);
		if (ret != 0) {
			printf("av_opt_set preset failed\n");
		}

		ret = av_opt_set(codec_ctx->priv_data, "profile", "main", 0); // 默认high
		if (ret != 0) {
			printf("av_opt_set profile failed\n");
		}

		ret = av_opt_set(codec_ctx->priv_data, "tune", "zerolatency", 0);
		if (ret != 0) {
			printf("av_opt_set tune failed\n");
		}
	}

	codec_ctx->bit_rate = 3000000;
	/*
		codec_ctx->rc_max_rate = 3000000;
		codec_ctx->rc_min_rate = 3000000;
	    codec_ctx->rc_buffer_size = 2000000;
	    codec_ctx->thread_count = 4;  // 开了多线程后也会导致帧输出延迟, 需要缓存thread_count帧后再编程。
	    codec_ctx->thread_type = FF_THREAD_FRAME;
	
		对于H264 AV_CODEC_FLAG_GLOBAL_HEADER  设置则只包含I帧，此时sps pps需要从codec_ctx->extradata读取
		不设置则每个I帧都带 sps pps sei	
		codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; // 存本地文件时不要去设置
	*/
	ret = avcodec_open2(codec_ctx, codec, NULL);
	ASSERT_TRUE(ret >= 0);

	printf("thread_count: %d, thread_type: %d\n",
		codec_ctx->thread_count, codec_ctx->thread_type);

	in_fp = fopen(in_yuv_file, "rb");
	ASSERT_TRUE(in_fp != NULL);

	out_fp = fopen(out_h264_file, "wb");
	ASSERT_TRUE(out_fp != NULL);

	pkt = av_packet_alloc();
	ASSERT_TRUE(pkt != NULL);

	frame = av_frame_alloc();
	ASSERT_TRUE(frame != NULL);

	frame->format = codec_ctx->pix_fmt;
	frame->width = codec_ctx->width;
	frame->height = codec_ctx->height;
	
	ret = av_frame_get_buffer(frame, 0);
	ASSERT_TRUE(ret >= 0);

	int frame_bytes = av_image_get_buffer_size((AVPixelFormat)frame->format,
		frame->width, frame->height, 1);
	printf("frame_bytes %d\n", frame_bytes);

	uint8_t* yuv_buf = (uint8_t*)malloc(frame_bytes);
	ASSERT_TRUE(yuv_buf != NULL);

	int64_t begin_time = get_time();
	int64_t end_time = begin_time;
	int64_t all_begin_time = get_time();
	int64_t all_end_time = all_begin_time;
	int64_t pts = 0;

	for (;;) {
		memset(yuv_buf, 0, frame_bytes);
		int read_bytes = fread(yuv_buf, 1, frame_bytes, in_fp);
		if (read_bytes <= 0) {
			break;
		}

		/* 
			确保该frame可写, 如果编码器内部保持了内存参考计数，则需要重新拷贝一个备份
			目的是新写入的数据和编码器保存的数据不能产生冲突
		*/
		int frame_is_writable = 1;
		if (av_frame_is_writable(frame) == 0) {
			printf("the frame can't write, buf:%p\n", frame->buf[0]);
			if (frame->buf && frame->buf[0]) {
				printf("ref_count2(frame) = %d\n", av_buffer_get_ref_count(frame->buf[0]));
			}
			frame_is_writable = 0;
		}

		ret = av_frame_make_writable(frame);
		if (av_frame_is_writable(frame) == 0) {
			printf("the frame can't write, buf:%p\n", frame->buf[0]);
			if (frame->buf && frame->buf[0]) {
				printf("ref_count2(frame) = %d\n", av_buffer_get_ref_count(frame->buf[0]));
			}
		}

		if (ret != 0) {
			break;
		}

		int need_size = av_image_fill_arrays(frame->data, frame->linesize, yuv_buf,
			(AVPixelFormat)frame->format, frame->width, frame->height, 1);
		if (need_size != frame_bytes) {
			printf("av_image_fill_arrays failed, need_size:%d, frame_bytes:%d\n",
				need_size, frame_bytes);
			break;
		}

		pts += 40;
		frame->pts = pts;
		begin_time = get_time();
		ret = encode(codec_ctx, frame, pkt, out_fp);
		end_time = get_time();
		printf("encode time:%lldms\n", end_time - begin_time);

		if (ret < 0) {
			printf("encode failed\n");
		}
	}

	encode(codec_ctx, NULL, pkt, out_fp);
	all_end_time = get_time();
	printf("all encode time:%lldms\n", all_end_time - all_begin_time);
	fclose(in_fp);
	fclose(out_fp);

	if (yuv_buf) {
		free(yuv_buf);
	}

	av_frame_free(&frame);
	av_packet_free(&pkt);
	avcodec_free_context(&codec_ctx);
}
#endif