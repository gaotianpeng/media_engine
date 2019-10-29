
#include <iostream>
#include <string.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
}

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

using namespace std; 
int main(int argc, char* argv[])
{
	AVFormatContext	*fmt_ctx = NULL;
	int				i = 0, audioStream = 0;
	AVCodecContext	*cctx = NULL;
	AVCodec			*codec = NULL;
	AVPacket		*pkt = NULL;
	uint8_t			*out_buffer = NULL;
	AVFrame			*frame = NULL;
	int ret;
	uint32_t len = 0;
	int got_picture = 0;
	int index = 0;
	int64_t in_channel_layout;
	struct SwrContext *au_convert_ctx = NULL;

	FILE *pFile = fopen("e:/ffmpeg/outputtest.pcm", "wb");
	char url[] = "e:/ffmpeg/src.aac";

	fmt_ctx = avformat_alloc_context();
	if (avformat_open_input(&fmt_ctx, url, NULL, NULL) != 0) {
		cout << "avformat_open_input() failed!!!" << endl; 
		return -1;
	}
	if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
		cout << "avformat_find_stream_info() failed!!!" << endl;
		return -1;
	}
	av_dump_format(fmt_ctx, 0, url, false);

	audioStream = -1;
	for (i = 0; i < fmt_ctx->nb_streams; i++) {
		if (fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
			audioStream = i;
			break;
		}
	}

	if (audioStream == -1) {
		cout << "failed to find audio stream!!!" << endl;
		return -1;
	}

	cctx = fmt_ctx->streams[audioStream]->codec;
	codec = avcodec_find_decoder(cctx->codec_id);
	if (!codec) {
		cout << "avcodec_find_decoder() failed!!!" << endl;
		return -1;
	}

	if (avcodec_open2(cctx, codec, NULL) < 0) {
		cout << "avcodec_open2(cctx, codec, NULL) failed!!!" << endl;
		return -1;
	}

	pkt = (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(pkt);

	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
	int out_nb_samples = cctx->frame_size;
	AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = 44100;
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
	int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);

	out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
	frame = av_frame_alloc();

	in_channel_layout = av_get_default_channel_layout(cctx->channels);
	au_convert_ctx = swr_alloc();
	au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,
		in_channel_layout, cctx->sample_fmt, cctx->sample_rate, 0, NULL);
	swr_init(au_convert_ctx);

	while (av_read_frame(fmt_ctx, pkt) >= 0) {
		if (pkt->stream_index == audioStream) {

			ret = avcodec_decode_audio4(cctx, frame, &got_picture, pkt);
			if (ret < 0) {
				cout << "avcodec_decode_audio4() failed!!!" << endl; 
				return -1;
			}
			if (got_picture > 0) {
				swr_convert(au_convert_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)frame->data, frame->nb_samples);
				printf("index:%5d\t pts:%lld\t packet size:%d\n", index, pkt->pts, pkt->size);
				fwrite(out_buffer, 1, out_buffer_size, pFile);
				index++;
			}
		}
		av_free_packet(pkt);
	}

	swr_free(&au_convert_ctx);
	fclose(pFile);
	av_free(out_buffer);
	avcodec_close(cctx);
	avformat_close_input(&fmt_ctx);
	return 0;
}
