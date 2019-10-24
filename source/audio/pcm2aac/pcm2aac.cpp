extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/avutil.h"
}

#include <iostream>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

static void encode(AVFormatContext* out_fmt, AVCodecContext *ctx, AVFrame *frame, AVPacket *pkt); 

int main(int argc, char* argv[]) {
	cout << "pcm2aac starting ..."<< endl; 
	char infile[] = "e:/ffmpeg/out.pcm";
	char outfile[] = "e:/ffmpeg/out.aac";

	av_register_all();
	avcodec_register_all();

	//libfdk_aac
	AVCodec *a_codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
	//AVCodec *a_codec = avcodec_find_encoder_by_name("libfdk_aac");
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
	a_ctx->thread_count = 4; 
	AVDictionary *opt = NULL;
	//aac_he, aac_ld
	int ret = av_dict_set(&opt, "profile", "aac_ld", 1);
	if (ret < 0) {
		cout << "av_dict_set(&opt, \"profile\", \"aac_he\", 0) failed!!!" << endl;
		return ret;
	}

	ret = av_dict_set(&opt, "profile", "aac_ld", 0);
	if (ret < 0) {
		cout << "av_dict_set(&opt, \"profile\", \"LD\", 0); failed!!!" << endl;
		return ret;
	}

	ret = avcodec_open2(a_ctx, a_codec, NULL);
	if (ret < 0) {
		cout << "avcodec_open2 error" << endl;
		getchar();
		return -1;
	}
	cout << "avcodec_open2 success!" << endl;

	AVFormatContext *out_fmt = NULL;
	avformat_alloc_output_context2(&out_fmt, NULL, NULL, outfile);
	if (!out_fmt) {
		cout << "avformat_alloc_output_context2 error" << endl;
		return -1;
	}

	AVStream *st = avformat_new_stream(out_fmt, NULL);
	st->codecpar->codec_tag = 0;
	avcodec_parameters_from_context(st->codecpar, a_ctx);
	av_dump_format(out_fmt, 0, outfile, 1);

	ret = avio_open(&out_fmt->pb, outfile, AVIO_FLAG_WRITE);
	if (ret < 0) {
		cout << "avio_open error" << endl;
		return -1;
	}
	ret = avformat_write_header(out_fmt, NULL);

	SwrContext *actx = NULL;
	actx = swr_alloc_set_opts(actx,
		a_ctx->channel_layout, a_ctx->sample_fmt, a_ctx->sample_rate,	//输出格式
		AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,		//输入格式
		0, 0);
	if (!actx) {
		cout << "swr_alloc_set_opts error" << endl;
		getchar();
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
	bool bFirst = true; 
	const uint8_t *data[1];
	data[0] = (uint8_t*)pcm;

	AVPacket pkt;
	av_init_packet(&pkt);

	TickMeter meter; 
	for ( ; ;) {
		int len = fread(pcm, 1, readSize, fp);
		if (len <= 0) {
			break;
		}

		meter.reset(); 
		meter.start(); 
		len = swr_convert(actx, frame->data, frame->nb_samples,
			data, frame->nb_samples);

		if (len <= 0) {
			break;
		}
		meter.stop(); 
		//cout << "swr_convert cost:" << meter.getTimeMilli()<< endl; 

		encode(out_fmt, a_ctx, frame, &pkt);
	}

	encode(out_fmt, a_ctx, nullptr, &pkt);
	delete pcm;
	pcm = NULL;
	av_write_trailer(out_fmt);
	avio_close(out_fmt->pb);
	avformat_free_context(out_fmt);
	avcodec_close(a_ctx);
	avcodec_free_context(&a_ctx);

	cout << "pcm2aac end." << endl;
	return 0;
}

static bool bFirst = true; 

static void encode(AVFormatContext* out_fmt, AVCodecContext *ctx, AVFrame *frame, AVPacket *pkt) {
	int ret;
	TickMeter time_meter;
	if (bFirst) {
		time_meter.reset();
		time_meter.start();
	}
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

		ret = av_interleaved_write_frame(out_fmt, pkt);
		av_packet_unref(pkt);
		if (bFirst) {
			time_meter.stop();
			cout << "---------------------" << time_meter.getTimeMilli() << endl;
			bFirst = false;
		}
	}
}
