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

static int check_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt); 
static int select_sample_rate(const AVCodec *codec); 
static int select_channel_layout(const AVCodec *codec); 
static void encode(AVFormatContext* out_fmt, AVCodecContext *ctx, AVFrame *frame, AVPacket *pkt);

int main(int argc, char* argv[]) {
	cout << "pcm2opus starting..." << endl; 
	const char* in_file = "e:/ffmpeg/out.pcm";
	const char* out_file = "e:/ffmpeg/out.opus"; 

	const AVCodec* a_codec = avcodec_find_encoder(AV_CODEC_ID_OPUS);
	if (!a_codec) {
		cout << "avcodec_find_encoder(AV_CODEC_ID_OPUS) failed!!!" << endl; 
		return -1; 
	}

	cout << "libopus encoder name is " << a_codec->name << endl; 
	
	AVCodecContext *a_ctx = avcodec_alloc_context3(a_codec);
	if (!a_ctx) {
		cout << "avcodec_alloc_context3(a_codec) failed!!!" << endl;
		return -1;
	}
	
	a_ctx->bit_rate = 64000; 
	a_ctx->sample_fmt = AV_SAMPLE_FMT_S16; 
	if (!check_sample_fmt(a_codec, a_ctx->sample_fmt)) {
		fprintf(stderr, "Encoder does not support sample format %s",
			av_get_sample_fmt_name(a_ctx->sample_fmt));
		return -1; 
	}
	a_ctx->sample_rate = select_sample_rate(a_codec);
	a_ctx->channel_layout = select_channel_layout(a_codec);
	a_ctx->channels = av_get_channel_layout_nb_channels(a_ctx->channel_layout);

	AVDictionary *opt = NULL;
	int ret = av_dict_set(&opt, "application", "lowdelay", 0);
	if (ret < 0) {
		cout << "av_dict_set profile failed!!!" << endl;
		return ret;
	}

	ret = avcodec_open2(a_ctx, a_codec, &opt);
	if (ret < 0) {
		cout << "avcodec_open2(a_ctx, a_codec, nullptr) failed!!!" << endl; 
		return ret; 
	}

	//packet for holding encoded output
	AVPacket* pkt = av_packet_alloc(); 
	if (!pkt) {
		cout << "av_packet_alloc() failed!!!" << endl; 
		return -1; 
	}

	//frame containing input raw audio 
	AVFrame* frame = av_frame_alloc(); 
	if (!frame) {
		fprintf(stderr, "av_frame_alloc\n");
		return -1; 
	}

	frame->nb_samples = a_ctx->frame_size;
	cout << "libopus encoder's frame size is " << a_ctx->frame_size << endl; 
	frame->format = a_ctx->sample_fmt;
	frame->channel_layout = a_ctx->channel_layout;
	//frame->channels = 2; 

	ret = av_frame_get_buffer(frame, 0);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate audio data buffers\n");
		exit(1);
	}

	AVFormatContext* out_fmt = NULL; 
	ret = avformat_alloc_output_context2(&out_fmt, NULL, NULL, out_file);
	if (ret < 0) {
		cout << "avformat_alloc_output_context2(&out_fmt, NULL, NULL, out_file) failed!!!" << endl; 
		return ret; 
	}

	AVStream* a_st = avformat_new_stream(out_fmt, NULL); 
	a_st->codecpar->codec_tag = 0; 
	ret = avcodec_parameters_from_context(a_st->codecpar, a_ctx); 
	if (ret < 0 ) {
		cout << "avcodec_parameters_from_context(a_st->codecpar, a_ctx) failed!!!" << endl;
		return ret;
	}
	av_dump_format(out_fmt, 0, out_file, 1); 
	ret = avio_open(&out_fmt->pb, out_file, AVIO_FLAG_WRITE);
	if (ret < 0) {
		cout << " avio_open(&out_fmt->pb, out_file, AVIO_FLAG_WRITE) failed!!!" << endl; 
		return ret; 
	}
	ret = avformat_write_header(out_fmt, NULL);
	if (ret < 0) {
		cout << "avformat_write_header(out_fmt, NULL) failed!!!" << endl;
		return ret;
	}

	int read_size = frame->nb_samples * 2 * 2; 
	char *pcm = new char[read_size];
	uint8_t *data[1];
	data[0] = (uint8_t*)pcm;
	frame->data[0] = data[0]; 
	FILE *fp = fopen(in_file, "rb");
	if (!fp) {
		cout << "fopen(in_file, \"rb\") failed!!!" << endl;
		return -1; 
	}

	for (; ;) {
		int len = fread(pcm, 1, read_size, fp); 
		if (len <= 0) {
			break; 
		}

		encode(out_fmt, a_ctx, frame, pkt); 
	}

	encode(out_fmt, a_ctx, nullptr, pkt);
	
	av_write_trailer(out_fmt);
	delete pcm; 
	pcm = nullptr; 
	avio_close(out_fmt->pb); 
	av_frame_free(&frame); 
	av_packet_free(&pkt);
	avcodec_free_context(&a_ctx); 

	cout << "pcm2opus end." << endl; 
	return 0;
}

/* check that a given sample format is supported by the encoder */
static int check_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt) {
	const enum AVSampleFormat *p = codec->sample_fmts;

	while (*p != AV_SAMPLE_FMT_NONE) {
		if (*p == sample_fmt)
			return 1;
		p;
	}
	return 0;
}

static int select_sample_rate(const AVCodec *codec) {
	const int *p;
	int best_samplerate = 0;

	if (!codec->supported_samplerates)
		return 44100;

	p = codec->supported_samplerates;
	while (*p) {
		if (!best_samplerate || abs(44100 - *p) < abs(44100 - best_samplerate))
			best_samplerate = *p;
		p;
	}
	return best_samplerate;
}

static int select_channel_layout(const AVCodec *codec) {
	const uint64_t *p;
	uint64_t best_ch_layout = 0;
	int best_nb_channels = 0;

	if (!codec->channel_layouts)
		return AV_CH_LAYOUT_STEREO;

	p = codec->channel_layouts;
	while (*p) {
		int nb_channels = av_get_channel_layout_nb_channels(*p);

		if (nb_channels > best_nb_channels) {
			best_ch_layout = *p;
			best_nb_channels = nb_channels;
		}
		p;
	}
	return best_ch_layout;
}

static void encode(AVFormatContext* out_fmt, AVCodecContext *ctx, AVFrame *frame, AVPacket *pkt) {
	int ret;
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
	}
}
