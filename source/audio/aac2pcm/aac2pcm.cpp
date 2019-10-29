extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
}

#include <iostream>
using namespace std; 

int main(int argc, char* argv[]) {
	cout << "aac2pcm starting..." << endl; 
	avcodec_register_all();
	const char* in_file = "e:/ffmpeg/src.aac"; 
	const char* out_file = "e:/ffmpeg/test.pcm"; 
	AVFormatContext* fmt = NULL; 
	int ret = avformat_open_input(&fmt, in_file, NULL, NULL); 
	if (ret < 0) {
		cout << "avformat_open_input(&fmt, in_file, NULL, NULL) failed!!!" << endl; 
		return -1; 
	}

	ret = avformat_find_stream_info(fmt, NULL); 
	if (ret < 0) {
		cout << "avformat_find_stream_info(fmt, NULL) failed!!!" << endl;
		return ret;
	}
	int audio_index = -1; 
	for (int i = 0; i < fmt->nb_streams; i++) {
		if (fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			audio_index = i; 
		}
	}
	AVCodec* codec_dec = avcodec_find_decoder(fmt->streams[audio_index]->codec->codec_id);
	if (!codec_dec) {
		cout << "avcodec_find_decoder(fmt->streams[audio_index]->codec->codec_id) failed!!!" << endl; 
		return -1; 
	}
	
	AVCodecContext* dec_ctx = avcodec_alloc_context3(codec_dec); 
	ret = avcodec_parameters_to_context(dec_ctx, fmt->streams[audio_index]->codecpar);
	if (ret < 0) {
		cout << "avcodec_parameters_to_context(dec_ctx, fmt->streams[audio_index]->codecpar) failed!!!" << endl;
		return ret;
	}

	ret = avcodec_open2(dec_ctx, codec_dec, NULL); 
	if (ret < 0) {
		cout << "avcodec_open2(dec_ctx, codec_dec, NULL) failed!!!" << endl; 
		return ret; 
	}

	AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO; 
	int out_nb_samples = dec_ctx->frame_size; 
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout); 
	int out_buf_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1); 
	int out_sample_rate = 44100; 
	int64_t in_channel_layout = av_get_default_channel_layout(dec_ctx->channels);

	SwrContext* a_swr_ctx = swr_alloc();
	a_swr_ctx = swr_alloc_set_opts(a_swr_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,
		in_channel_layout, dec_ctx->sample_fmt, dec_ctx->sample_rate, 0, NULL);

	swr_init(a_swr_ctx);
	uint8_t** out_data = NULL; 
	int out_linesize = 0; 
	ret = av_samples_alloc_array_and_samples(&out_data, &out_linesize, out_channels,
		out_nb_samples, out_sample_fmt, 0);
	if (ret < 0) {
		cout << "av_samples_alloc_array_and_samples failed!!!" << endl; 
		return -1; 
	}
	int bufsize = 0; 
	AVFrame* frame = av_frame_alloc();
	AVPacket* pkt = av_packet_alloc();
	av_init_packet(pkt);
	if (!frame || !pkt) {
		cout << "failed to init pkt or frame data!!!" << endl;
		return -1;
	}


	FILE* p_out = fopen(out_file, "wb");
	while (true) {
		ret = av_read_frame(fmt, pkt);
		if (ret < 0) {
			cout << "av_read_frame(fmt, pkt) failed!!!" << endl; 
 			break;
		} 

		ret = avcodec_send_packet(dec_ctx, pkt);
		if (ret < 0) {
			cout << "avcodec_send_packet(dec_ctx, pkt) failed!!!" << endl;
			break;
		}
		cout << " avcodec_send_packet success." << endl;
		while (ret >= 0) {
			ret = avcodec_receive_frame(dec_ctx, frame);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				cout << "avcodec_receive_frame AVERROR AVERROR_EOF" << endl;
				return -1;
			}
			else if (ret < 0) {
				cout << "error during decoding!!!" << endl;
				return -1;
			}
		}
	}

	avformat_close_input(&fmt); 
	cout << "aac2pcm end!!" << endl; 
	return 0; 
}