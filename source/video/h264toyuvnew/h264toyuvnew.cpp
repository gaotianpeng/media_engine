#include <iostream>
using namespace std;

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libavfilter/avfilter.h"
#include "libswscale/swscale.h"
}

#include "opencv2/opencv.hpp"

using namespace cv; 

int main(int argc, char* argv[]) {
	cout << "h264toyuvnew starting ..." << endl; 
	const char* in_file = "e:/ffmpeg/720p.h264"; 
	const char* out_file = "e:/ffmpeg/720pnew.yuv"; 

	avcodec_register_all(); 
	int ret = -1; 
	AVFormatContext* v_fmt = nullptr;
	ret = avformat_open_input(&v_fmt, in_file, nullptr, nullptr); 
	if (ret < 0) {
		cout << "avformat_open_input(&v_fmt, in_file, nullptr, nullptr) failed!!!" << endl; 
		return ret; 
	}

	ret = avformat_find_stream_info(v_fmt, nullptr); 
	if (ret < 0) {
		cout << "avformat_find_stream_info(v_fmt, nullptr) failed!!!" << endl; 
		return ret; 
	}

	av_dump_format(v_fmt, -1, in_file, 0);

	ret = -1; 
	int video_index = -1; 
	for (int i = 0; i < v_fmt->nb_streams; i) {
		if (v_fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			video_index = i; 
		}
	}
	if (video_index == -1) {
		cout << "Failed to find an video stream!!!" << endl; 
		return ret; 
	}

	//AVCodecParserContext* v_parser = nullptr;
	//v_parser = av_parser_init(v_codec->id); 
	//if (!v_parser) {
	//	cout << "av_parser_init(v_codec->id) failed!!!" << endl; 
	//	return ret; 
	//}

	AVCodecContext* v_ctx = nullptr;
	v_ctx = avcodec_alloc_context3(nullptr); 
	if (!v_ctx) {
		cout << "avcodec_alloc_context3(v_codec) failed!!!" << endl;
		return ret; 
	}

	ret = avcodec_parameters_to_context(v_ctx, v_fmt->streams[video_index]->codecpar);
	if (ret < 0) {
		cout << "avcodec_parameters_to_context(v_ctx, v_fmt->streams[video_index]->codecpar) failed!!!" << endl; 
 		return ret; 
	}
	v_ctx->thread_count = 8; 
	ret = -1;
	AVCodec* v_codec = nullptr;
	v_codec = avcodec_find_decoder(v_ctx->codec_id);
	if (!v_codec) {
		cout << "avcodec_find_encoder(AV_CODEC_ID_H264) failed!!!" << endl;
		return ret;
	}
	//v_ctx->time_base.num = 1; 
	//v_ctx->time_base.den = 25;
	//low_delay
	AVDictionary *opt = NULL;
	ret = av_dict_set_int(&opt, "low_latency", 1, 0);
	if (ret < 0) {
		cout << "av_dict_set_int(&opt, \"low_latency\", 1, 0)" << endl; 
		return -1; 
	}
	ret = -1; 
	ret = avcodec_open2(v_ctx, v_codec, &opt); 
	if (ret < 0) {
		cout << "avcodec_open2(v_ctx, v_codec, nullptr) failed!!!" << endl; 
		return ret; 
	}

	AVPacket* pkt = nullptr;
	pkt = av_packet_alloc();
	if (!pkt) {
		cout << "av_packet_alloc() failed!!!" << endl;
		return ret;
	}

	AVFrame* pframe = nullptr;
	pframe = av_frame_alloc();
	if (!pframe) {
		cout << "av_frame_alloc() failed!!!" << endl;
		return ret;
	}

	AVFrame* pYUVFrame = nullptr;
	pYUVFrame = av_frame_alloc();
	if (!pYUVFrame) {
		cout << "av_frame_alloc() failed!!!" << endl;
		return ret;
	}


	struct SwsContext* img_convert_ctx = nullptr; 
	img_convert_ctx = sws_getContext(v_ctx->width, v_ctx->height,
		v_ctx->pix_fmt,
		v_ctx->width,
		v_ctx->height,
		AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	if (!img_convert_ctx) {
		cout << "sws_getContext() failed!!!" << endl; 
		return ret; 
	}

	int video_size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P,
		v_ctx->width, v_ctx->height, 1);
	uint8_t* buf = (uint8_t*)av_malloc(video_size); 
	ret = av_image_fill_arrays(pYUVFrame->data, pYUVFrame->linesize, buf, 
				AV_PIX_FMT_YUV420P, v_ctx->width, v_ctx->height, 1);
	if (ret < 0) {
		cout << "av_image_fill_arrays() failed!!!" << endl; 
		return ret; 
	}
	ret = -1; 
	FILE* fp = fopen(out_file, "wb"); 
	if (!fp) {
		cout << "fopen(out_file, \"wb\")" << endl;
		return ret; 
	}

	ret = -1; 
	int ysize = v_ctx->height * v_ctx->width; 
	TickMeter time_meter; 
	while (av_read_frame(v_fmt, pkt) >= 0) {
		if (pkt->stream_index == video_index) {
			time_meter.reset(); 
			time_meter.start(); 
			ret = avcodec_send_packet(v_ctx, pkt);
			if (ret < 0) {
				cout << "avcodec_send_packet(v_ctx, pkt) failed!!!" << endl; 
				return ret; 
			}

			ret = avcodec_receive_frame(v_ctx, pframe); 
			if (AVERROR(EAGAIN) == ret) {
				cout << "avcodec_receive_frame failed!!! ret=" << ret << "continuie" << endl;
				continue;
			}

			if (ret != 0) {
				cout << "avcodec_receive_frame failed ret=" << ret << endl;
				return ret;
			}
			//cout << "decoding frame" << v_ctx->frame_number << endl;
			time_meter.stop();
			cout << "-------------------------cost:" << time_meter.getTimeMilli() << endl;

			ret = sws_scale(img_convert_ctx, pframe->data, pframe->linesize,
				0, v_ctx->height, pYUVFrame->data, pYUVFrame->linesize); 
			if (ret <= 0) {
				cout << "sws_scale failed!!!" << endl; 
				return ret; 
			}

			fwrite(pYUVFrame->data[0], 1, ysize, fp);//Y
			fwrite(pYUVFrame->data[1], 1, ysize / 4, fp);//U
			fwrite(pYUVFrame->data[2], 1, ysize / 4, fp);//V
		}
	}
 
	av_free(buf);
	avcodec_close(v_ctx); 
	//av_parser_close(v_parser); 
	avcodec_free_context(&v_ctx); 
	av_packet_free(&pkt); 
	av_frame_free(&pframe); 
	av_frame_free(&pYUVFrame); 
	avformat_close_input(&v_fmt);
	cout << "h264toyuvnew end." << endl;
	return 0;
}
