extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/avutil.h"
}

#include <iostream>
using namespace std;
#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

static void encode(AVFormatContext* out_fmt, AVCodecContext *ctx, AVFrame *frame, AVPacket *pkt);

int main(int argc, char* argv[]) {
	cout << "pcm2aac starting ..." << endl;
	char infile[] = "e:/ffmpeg/src.pcm";
	char outfile[] = "e:/ffmpeg/ldpcmno.pcm";
	FILE* pFile = fopen(outfile, "wb");
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
	a_ctx->thread_count = 4;
	//a_ctx->profile = FF_PROFILE_AAC_ELD;
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

	AVPacket* pkt = av_packet_alloc(); 
	av_init_packet(pkt);
	int got_picture = 0;

	//////////////////////////////////////////////////////
	AVCodec* dec_codex = avcodec_find_decoder(AV_CODEC_ID_AAC);
	AVCodecContext* dec_ctx = avcodec_alloc_context3(dec_codex);
	dec_ctx->bit_rate = 64000;
	dec_ctx->sample_rate = 44100;
	dec_ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
	dec_ctx->channel_layout = AV_CH_LAYOUT_STEREO;
	dec_ctx->channels = 2;
	dec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	dec_ctx->thread_count = 4;
	//dec_ctx->profile = FF_PROFILE_AAC_ELD;
	struct SwrContext *au_convert_ctx = NULL;
	au_convert_ctx = swr_alloc();
	au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,
		AV_CH_LAYOUT_STEREO, dec_ctx->sample_fmt, dec_ctx->sample_rate, 0, NULL);
	swr_init(au_convert_ctx);
	AVFrame	*frame_out = NULL;
	frame_out = av_frame_alloc();
	uint8_t	*out_buffer = NULL;
	out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
	int index = 0;

	ret = avcodec_open2(dec_ctx, dec_codex, NULL);
	if (ret < 0) {
		cout << "avcodec_open2 decoder error" << endl;
		getchar();
		return -1;
	}

	cout << "avcodec_open2 success!" << endl;
	for (; ;) {
		if (fread(pcm, 1, readSize, fp) <= 0) {
			break;
		}

		if (swr_convert(actx, frame->data, frame->nb_samples,
			data, frame->nb_samples) <= 0) {
			break;
		}

		int ret = -1;
		ret = avcodec_send_frame(a_ctx, frame);
		if (ret < 0) {
			fprintf(stderr, "Error sending the frame to the encoder\n");
			exit(1);
		}

		while (ret >= 0) {
			ret = avcodec_receive_packet(a_ctx, pkt);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				break;
			}
			else if (ret < 0) {
				fprintf(stderr, "Error encoding audio frame\n");
				exit(1);
			}

			ret = avcodec_send_frame(a_ctx, frame);
			if (ret < 0) {
				fprintf(stderr, "Error sending the frame to the encoder\n");
				exit(1);
			}

			while (ret >= 0) {
				ret = avcodec_receive_packet(a_ctx, pkt);
				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
					break;
				}
				else if (ret < 0) {
					fprintf(stderr, "Error encoding audio frame\n");
					exit(1);
				}

				pkt->stream_index = 0;
				pkt->pts = 0;
				pkt->dts = 0;
				cout << pkt->size << endl;

				ret = avcodec_decode_audio4(dec_ctx, frame_out, &got_picture, pkt);
				if (ret < 0) {
					cout << "avcodec_decode_audio4() failed!!!" << endl;
					return -1;
				}
				if (got_picture > 0) {
					swr_convert(au_convert_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)frame_out->data, frame_out->nb_samples);
					printf("index:%5d\t pts:%lld\t packet size:%d\n", index, pkt->pts, pkt->size);
					fwrite(out_buffer, 1, 4096, pFile);
					index++;
				}
				av_packet_unref(pkt);
			}
		}
	}

	delete pcm;
	pcm = NULL;
	avcodec_close(a_ctx);
	avcodec_free_context(&a_ctx);
	fclose(pFile);
	cout << "pcm2aac end." << endl;
	return 0;
}

static void encode(AVFormatContext* out_fmt, AVCodecContext *ctx, AVFrame *frame, AVPacket *pkt) {
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
		cout << pkt->size << endl; 
		//ret = av_interleaved_write_frame(out_fmt, pkt);
		av_packet_unref(pkt);
	}
}
