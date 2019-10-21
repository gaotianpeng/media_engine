#include <iostream>
#include <chrono>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavutil/avutil.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
}

using namespace std; 
using namespace std::chrono;

void encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt,
	FILE* out_file);

int main(int argc, char* argv[]) {
	cout << "yuvtoh264 new starting ..." << endl;
		
	avcodec_register_all();
	const char* out_filename = "e:/ffmpeg/720p1.h264"; 
	const char* in_filename = "e:/ffmpeg/720p.yuv";
	AVCodec* codec = nullptr;
	codec = avcodec_find_encoder(AV_CODEC_ID_H264);
	if (!codec) {
		cout << "avcodec_find_encoder failed!!!" << endl; 
		return -1; 
	}
	AVCodecContext* cctx = nullptr; 
	cctx = avcodec_alloc_context3(codec);
	if (!cctx) {
		cout << "avcodec_alloc_context3 failed!!!" << endl; 
		return -1; 
	}
		
	cctx->bit_rate = 400000; 
	cctx->width = 1280; 
	cctx->height = 720; 
	cctx->time_base.num = 1; 
	cctx->time_base.den = 25; 
	cctx->framerate.num = 25; 
	cctx->framerate.den = 1; 	
	cctx->gop_size = 10; 
	cctx->pix_fmt = AV_PIX_FMT_YUV420P; 
	//if (codec->id == AV_CODEC_ID_H264) {
	//	av_opt_set(cctx->priv_data, "preset", "ultrafast", 0); 
	//	av_opt_set(cctx->priv_data, "tune", "zerolatency", 0); 
	//}

	int ret = -1;
	// …Ë÷√±‡¬Î∆˜≤Œ ˝
	AVDictionary *opt = NULL;
	ret = av_dict_set(&opt, "profile", "baseline", 0);
	ret = av_dict_set(&opt, "preset", "ultrafast", 0);
	ret = av_dict_set(&opt, "tune", "zerolatency", 0);
	if (ret < 0) {
		cout << "av_dict_set profile failed!!!" << endl;
		return ret;
	}

	ret = avcodec_open2(cctx, codec, &opt); 
	//ret = avcodec_open2(cctx, codec, NULL); 
	if (ret < 0) {
		cout << "avcodec_open2 failed!!!" << endl; 
		return ret; 
	}

	AVFrame* pframe = nullptr; 
	pframe = av_frame_alloc();
	if (!pframe) {
		cout << "av_frame_alloc video frame failed!!!" << endl;
		return -1; 
	}
	pframe->format = cctx->pix_fmt; 
	pframe->width = cctx->width; 
	pframe->height = cctx->height;
	
	ret = av_image_alloc(pframe->data, pframe->linesize, pframe->width, pframe->height, cctx->pix_fmt, 16); 
	if (ret < 0) {
		cout << "av_image_alloc video frame failed!!!" << endl;
		return -1;
	}

	FILE* p_infile = fopen(in_filename, "rb");
	if (!p_infile) {
		cout << "fopen(in_filename, \"rb\") failed!!!" << endl;
		return -1; 
	}

	FILE* p_outfile = fopen(out_filename, "wb"); 
	if (!p_outfile) {
		cout << "fopen(out_filename, \"wb\") failed!!!" << endl;
		return -1; 
	}
	int i = 0; 
	int y_size = cctx->width * cctx->height;

	AVPacket* pkt = nullptr; 
	pkt = av_packet_alloc();
	if (!pkt) {
		exit(1);
	}
		
	while (true) {
		if (fread(pframe->data[0], 1, y_size, p_infile) <= 0 ||		
			fread(pframe->data[1], 1, y_size/4, p_infile) <= 0 ||	
			fread(pframe->data[2], 1, y_size/4, p_infile) <= 0) {	
			cout << "Failed to read file data " << endl; 
			break;
		}

		if (feof(p_infile)) {
			steady_clock::duration d = steady_clock::now().time_since_epoch();

			break;
		}

		pframe->pts = i; 
		++i; 
		//cout << "frame i " << i << endl;

		encode(cctx, pframe, pkt, p_outfile);
	}

	encode(cctx, NULL, pkt, p_outfile); 

	fclose(p_infile);
	fclose(p_outfile); 
	av_frame_free(&pframe); 
	av_packet_free(&pkt); 
	avcodec_free_context(&cctx);
	cout << "yuvtoh264 new ended!" << endl;
	return 0; 
}

static bool bFirstEncode = true; 
static bool bFirstSend = true; 
static void encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt,
	FILE* out_file) {

	int ret = 0; 
	if (frame) {
		//cout << "send frame " << frame->pts << " to encoder." << endl; 
	}
	ret = avcodec_send_frame(enc_ctx, frame); 
	if (ret < 0) {
		cout << "avcodec_send_frame error!!!" << endl; 
		exit(1); 
	}

	steady_clock::duration start;
	steady_clock::duration end;
	if (bFirstSend) {
		start = steady_clock::now().time_since_epoch();
		milliseconds mil = duration_cast<milliseconds>(start);
		cout << "first frame encode start ms: " << mil.count() << endl;
	}
	bFirstSend = false; 

	while (ret >= 0) {
		ret = avcodec_receive_packet(enc_ctx, pkt); 
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			return; 
		}
		else if (ret < 0) {
			cout << "avcodec_receive_packet error!!!" << endl;
			exit(1); 
		}
		
		//cout << "Write packet " << pkt->pts << ", " << pkt->size << endl;

		if (bFirstEncode) {
			steady_clock::duration d = steady_clock::now().time_since_epoch();
			milliseconds mil = duration_cast<milliseconds>(d);
			cout << "first frame encode end ms: " << mil.count()  << endl;
		}
		bFirstEncode = false; 

		fwrite(pkt->data, 1, pkt->size, out_file); 
		av_packet_unref(pkt); 
	}

	return; 
}