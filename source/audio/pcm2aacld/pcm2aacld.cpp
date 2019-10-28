extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/avutil.h"
}

#include <iostream>
using namespace std;

static void encode(AVCodecContext *ctx, AVFrame *frame, AVPacket *pkt);

int main(int argc, char* argv[]) {
	cout << "pcm2aac starting ..." << endl;
	char infile[] = "e:/ffmpeg/src.pcm";

	av_register_all();
	avcodec_register_all();

	AVCodec *a_codec = avcodec_find_encoder(AV_CODEC_ID_AAC);

	if (!a_codec) {
		cout << "avcodec_find_encoder_by_name error" << endl;
		return -1;
	}
	AVCodecContext *a_ctx = avcodec_alloc_context3(a_codec);
	if (!a_ctx) {
		cout << "avcodec_alloc_context3 error" << endl;
		getchar();
		return -1;
	}
	a_ctx->bit_rate = 64000;
	a_ctx->sample_rate = 44100;
	a_ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
	a_ctx->channel_layout = AV_CH_LAYOUT_STEREO;
	a_ctx->channels = 2;
	a_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	a_ctx->profile = FF_PROFILE_AAC_ELD;
	a_ctx->thread_count = 4;

	int ret = -1; 
	ret = avcodec_open2(a_ctx, a_codec, NULL);
	if (ret < 0) {
		cout << "avcodec_open2 error" << endl;
		getchar();
		return -1;
	}
	cout << "avcodec_open2 success!" << endl;

	SwrContext *actx = NULL;
	actx = swr_alloc_set_opts(actx,
		a_ctx->channel_layout, a_ctx->sample_fmt, a_ctx->sample_rate,	//输出格式
		AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,		//输入格式
		0, 0);
	if (!actx) {
		cout << "swr_alloc_set_opts error" << endl;
		return -1;
	}
	ret = swr_init(actx);
	if (ret < 0) {
		cout << "swr_init error" << endl;
		getchar();
		return -1;
	}

	AVFrame *frame = av_frame_alloc();
	frame->format = AV_SAMPLE_FMT_FLTP;
	frame->channels = 2;
	frame->channel_layout = AV_CH_LAYOUT_STEREO;
	frame->nb_samples = 1024;
	ret = av_frame_get_buffer(frame, 0);
	if (ret < 0) {
		cout << "av_frame_get_buffer error" << endl;
		getchar();
		return -1;
	}

	int readSize = frame->nb_samples * 2 * 2;

	char *pcm = new char[readSize];
	FILE *fp = fopen(infile, "rb");
	if (!fp) {
		cout << "fopen(infile, \"rb\") failed!!!" << endl; 
		return -1; 
	}
	bool bFirst = true;
	const uint8_t *data[1];
	data[0] = (uint8_t*)pcm;

	AVPacket pkt;
	av_init_packet(&pkt);

	for (; ;) {
		int len = fread(pcm, 1, readSize, fp);
		if (len <= 0) {
			break;
		}

		len = swr_convert(actx, frame->data, frame->nb_samples,
			data, frame->nb_samples);

		if (len <= 0) {
			break;
		}
		
		encode( a_ctx, frame, &pkt);
	}

	encode(a_ctx, nullptr, &pkt);
	delete pcm;
	pcm = NULL;
	avcodec_close(a_ctx);
	avcodec_free_context(&a_ctx);

	cout << "pcm2aac end." << endl;
	return 0;
}

static void encode(AVCodecContext *ctx, AVFrame *frame, AVPacket *pkt) {
	int ret = -1;
	ret = avcodec_send_frame(ctx, frame);
	if (ret < 0) {
		fprintf(stderr, "Error sending the frame to the encoder\n");
		exit(1);
	}

	while (ret >= 0) {
		ret = avcodec_receive_packet(ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			return;
		}
		else if (ret < 0) {
			fprintf(stderr, "Error encoding audio frame\n");
			exit(1);
		}

		pkt->stream_index = 0;
		pkt->pts = 0;
		pkt->dts = 0;
		cout << "pkt size: " << pkt->size << endl; 
		av_packet_unref(pkt);
	}
}
