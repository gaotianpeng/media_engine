#include <iostream>
using namespace std;

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
}

AVFormatContext* AFCtx_p;//解封装上下文
AVCodecContext* ACCtx_p;//解码器上下文
AVCodec* codec_p;//解码器
AVFrame* pFrame;
AVFrame* pFrameYUV444;
AVPacket* packet;
struct SwsContext* img_convert_ctx;

int videoindex = -1;
int ret, got_picture = 0;
int y_size;

const char path[] = "e:/ffmpeg/720p.h264";

int main(int argc, char* argv[]) {
	if (avformat_open_input(&AFCtx_p, path, NULL, NULL) != 0) {
		cout << "open failed\n" << endl;
		return -1;
	} 
	if (avformat_find_stream_info(AFCtx_p, NULL) < 0) {
		cout << "dtream find failed\n" << endl;
		return -1;
	}
	av_dump_format(AFCtx_p, -1, path, 0);
	for (int i = 0; i < AFCtx_p->nb_streams; i) {
		if (AFCtx_p->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoindex = i;
		}
	}
	if (videoindex == -1) {
		cout << "not a video\n" << endl;
		return -1;
	}
	ACCtx_p = avcodec_alloc_context3(NULL);
	if (avcodec_parameters_to_context(ACCtx_p, AFCtx_p->streams[videoindex]->codecpar) < 0)	{
		cout << "copy stream failed\n" << endl;
		return -1;
	}

	codec_p = avcodec_find_decoder(ACCtx_p->codec_id);
	if (!codec_p) {
		cout << "find decoder error\n" << endl;
		return -1;
	}
	if (avcodec_open2(ACCtx_p, codec_p, NULL) != 0) {
		cout << "open codec failed" << endl;
		return -1;
	}

	img_convert_ctx = sws_getContext(ACCtx_p->width, ACCtx_p->height,
		ACCtx_p->pix_fmt,
		ACCtx_p->width, ACCtx_p->height, AV_PIX_FMT_YUV444P, SWS_BICUBIC, NULL, NULL, NULL);
	if (!img_convert_ctx)
	{
		cout << "get swscale context failed\n" << endl;
		return -1;
	}
	packet = av_packet_alloc();
	pFrame = av_frame_alloc();
	pFrameYUV444 = av_frame_alloc();
	//获取转换后YUV数据的大小
	int video_size = av_image_get_buffer_size(AV_PIX_FMT_YUV444P, ACCtx_p->width,
		ACCtx_p->height, 1);
	uint8_t* buf = (uint8_t*)av_malloc(video_size);
	av_image_fill_arrays(pFrameYUV444->data, pFrameYUV444->linesize,
		buf, AV_PIX_FMT_YUV444P, ACCtx_p->width, ACCtx_p->height, 1);
	char errbuf[256] = { 0 };
	FILE* fp_yuv = fopen("e:/ffmpeg/720p444.yuv", "wb");

	int ret = 0;
	//循环读取帧数据并转换写入
	while (av_read_frame(AFCtx_p, packet) >= 0)
	{
		if (packet->stream_index == videoindex)
		{
			if (avcodec_send_packet(ACCtx_p, packet) != 0)
			{
				cout << "send video stream packet failed\n" << endl;
				return -1;
			}
			ret = avcodec_receive_frame(ACCtx_p, pFrame);
			if (AVERROR(EAGAIN) == ret)
			{
				cout << "receive video frame failed ret=" << ret << "continuie" << endl;
				continue;
			}
			if (ret != 0)
			{
				cout << "receive video frame failed ret=" << ret << endl;
				return -1;
			}
			cout << "decoding frame" << ACCtx_p->frame_number << endl;

			sws_scale(img_convert_ctx,
				pFrame->data, pFrame->linesize,
				0, ACCtx_p->height, pFrameYUV444->data, pFrameYUV444->linesize);

			y_size = ACCtx_p->width * ACCtx_p->height;
			fwrite(pFrameYUV444->data[0], 1, y_size, fp_yuv);//Y
			fwrite(pFrameYUV444->data[1], 1, y_size, fp_yuv);//U
			fwrite(pFrameYUV444->data[2], 1, y_size, fp_yuv);//V
		}
	}
	fclose(fp_yuv);
	av_free(buf);
	av_frame_free(&pFrame);
	av_frame_free(&pFrameYUV444);
	av_packet_free(&packet);
	sws_freeContext(img_convert_ctx);
	avcodec_free_context(&ACCtx_p);
	avformat_close_input(&AFCtx_p);
	avformat_free_context(AFCtx_p);
	return 1;
}
