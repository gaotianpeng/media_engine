extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
}

#include <iostream>
using namespace std; 

int main(int argc, char* argv[]) {
	cout << "aac2pcm starting..." << endl; 

	const char* in_file = "e:/ffmpeg/out.aac"; 
	const char* out_file = "e:/ffmpeg/test.pcm"; 
	AVFormatContext* fmt = NULL; 
	int ret = avformat_open_input(&fmt, in_file, NULL, NULL); 
	if (ret < 0) {
		cout << "avformat_open_input(&fmt, in_file, NULL, NULL) failed!!!" << endl; 
		return -1; 
	}

	AVPacket* pkt = av_packet_alloc(); 
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

	int count = 0; 
	AVFrame* frame = av_frame_alloc(); 
	
	while (true) {
		ret = av_read_frame(fmt, pkt); 
		if (ret < 0) {
			break; 
		}
		//cout << "------------------------" << count++<< endl; 
	}


	avformat_close_input(&fmt); 
	cout << "aac2pcm end!!" << endl; 
	return 0; 
}