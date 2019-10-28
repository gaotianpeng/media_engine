#include <stdio.h>


#define __STDC_CONSTANT_MACROS


#ifdef _WIN32
//Windows
extern "C"
{
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
#include <libswscale/swscale.h>
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

#ifdef __cplusplus
};
#endif
#endif
#pragma comment(lib, "avcodec.lib")   
#pragma comment(lib, "avutil.lib")  
//test different codec
#define TEST_H264  1
#define TEST_HEVC  0




int main(int argc, char* argv[])
{
	AVCodec *pCodec;
	AVCodecContext *pCodecCtx = NULL;
	int i, ret, got_output;
	FILE *fp_in;
	FILE *fp_out;
	AVFrame *pFrame;
	AVFrame *pRgbFrame;
	AVPacket pkt;
	int y_size;
	int framecnt = 0;


	char filename_in[] = "e:/ffmpeg/720p.rgb";


#if TEST_HEVC
	AVCodecID codec_id = AV_CODEC_ID_HEVC;
	char filename_out[] = "ds.hevc";
#else
	AVCodecID codec_id = AV_CODEC_ID_H264;
	char filename_out[] = "e:/ffmpeg/rgb-ds.h264";
#endif




	int in_w = 1280, in_h = 720;
	int framenum = 10000000;


	avcodec_register_all();


	//pCodec = avcodec_find_encoder(codec_id);
	pCodec = avcodec_find_encoder_by_name("libx264");
	if (!pCodec) {
		printf("Codec not found\n");
		return -1;
	}
	pCodecCtx = avcodec_alloc_context3(pCodec);
	if (!pCodecCtx) {
		printf("Could not allocate video codec context\n");
		return -1;
	}
	pCodecCtx->bit_rate = 400000;
	pCodecCtx->width = in_w;
	pCodecCtx->height = in_h;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;
	pCodecCtx->gop_size = 10;
	pCodecCtx->max_b_frames = 1;
	pCodecCtx->pix_fmt = AV_PIX_FMT_RGB24;


	if (codec_id == AV_CODEC_ID_H264)
		av_opt_set(pCodecCtx->priv_data, "preset", "slow", 0);

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("Could not open codec\n");
		return -1;
	}

	pFrame = av_frame_alloc();
	if (!pFrame) {
		printf("Could not allocate video frame\n");
		return -1;
	}
	pFrame->format = pCodecCtx->pix_fmt;
	pFrame->width = pCodecCtx->width;
	pFrame->height = pCodecCtx->height;


	ret = av_image_alloc(pFrame->data, pFrame->linesize, pCodecCtx->width, pCodecCtx->height,
		pCodecCtx->pix_fmt, 16);
	if (ret < 0) {
		printf("Could not allocate raw picture buffer\n");
		return -1;
	}

	pRgbFrame = av_frame_alloc();
	if (!pRgbFrame) {
		printf("Could not allocate video frame\n");
		return -1;
	}
	pRgbFrame->format = AV_PIX_FMT_RGB24;
	pRgbFrame->width = pCodecCtx->width;
	pRgbFrame->height = pCodecCtx->height;


	ret = av_image_alloc(pRgbFrame->data, pRgbFrame->linesize, pCodecCtx->width, pCodecCtx->height,
		AV_PIX_FMT_RGB24, 24);
	if (ret < 0) {
		printf("Could not allocate raw picture buffer\n");
		return -1;
	}

	//Input raw data
	fp_in = fopen(filename_in, "rb");
	if (!fp_in) {
		printf("Could not open %s\n", filename_in);
		return -1;
	}
	//Output bitstream
	fp_out = fopen(filename_out, "wb");
	if (!fp_out) {
		printf("Could not open%s\n", filename_out);
		return -1;
	}

	SwsContext *sCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	y_size = pCodecCtx->width * pCodecCtx->height;
	//Encode
	for (i = 0; i < framenum; i) {
		av_init_packet(&pkt);
		pkt.data = NULL;    // packet data will be allocated by the encoder
		pkt.size = 0;

		//Read raw YUV data
		if (fread(pRgbFrame->data[0], pCodecCtx->width * pCodecCtx->height * 3, 1, fp_in) <= 0)
		{
			return -1;
		}
		else if (feof(fp_in)) {
			break;
		}
		sws_scale(sCtx, pRgbFrame->data, pRgbFrame->linesize, 0, pCodecCtx->height, pFrame->data, pFrame->linesize);
		pFrame->pts = i;
		/* encode the image */
		ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_output);
		if (ret < 0) {
			printf("Error encoding frame\n");
			return -1;
		}
		if (got_output) {
			printf("Succeed to encode frame: %5d\tsize:%5d\n", framecnt, pkt.size);
			framecnt;
			fwrite(pkt.data, 1, pkt.size, fp_out);
			av_free_packet(&pkt);
		}
	}
	//Flush Encoder
	for (got_output = 1; got_output; i) {
		ret = avcodec_encode_video2(pCodecCtx, &pkt, NULL, &got_output);
		if (ret < 0) {
			printf("Error encoding frame\n");
			return -1;
		}
		if (got_output) {
			printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", pkt.size);
			fwrite(pkt.data, 1, pkt.size, fp_out);
			av_free_packet(&pkt);
		}
	}


	fclose(fp_out);
	avcodec_close(pCodecCtx);
	av_free(pCodecCtx);
	av_freep(&pFrame->data[0]);
	av_frame_free(&pFrame);
	av_freep(&pRgbFrame->data[0]);
	av_frame_free(&pRgbFrame);

	return 0;
}
