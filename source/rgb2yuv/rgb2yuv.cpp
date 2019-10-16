#include <iostream>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libswscale/swscale.h" 
#include "libavutil/time.h"
};

//const char *srcFileName = "/home/ubuntu/gaotianpeng/arm_x86_anbox_test/720p.rgb";
//const char *dstFileName = "/home/ubuntu/gaotianpeng/arm_x86_anbox_test/720pp.yuv";

const char *srcFileName = "e:/ffmpeg/720p.rgb";
const char *dstFileName = "e:/ffmpeg/1280x720.yuv";

using namespace std;

int main(int argc, char* argv[]) {
	cout << "hello rgb2yuv started!" << endl;
	const int in_width = 1280;
	const int in_height = 720;
	const int out_width = 1280;
	const int out_height = 720;

	FILE *fin = fopen(srcFileName, "rb");
	FILE *fout = fopen(dstFileName, "wb");
	if (fin == NULL ) {
		fprintf(stderr, "open input file %s error.\n", srcFileName);
		return -1;
	}
	if (fout == NULL) {
		fprintf(stderr, "open output file %s error.\n", dstFileName);
		return -1;
	}

	const int read_size = in_width * in_height * 3;
	const int write_size = out_width * out_height * 3 / 2;
	uint8_t *ptr_src_rgb_buf = nullptr;
	uint8_t *ptr_dst_yuv_buf = nullptr;
	ptr_src_rgb_buf = new uint8_t[in_width * in_height * 3];
	ptr_dst_yuv_buf = new uint8_t[out_width * out_height * 3 / 2];

	uint8_t* inbuf = (uint8_t *)malloc(in_width*in_height*3);
	uint8_t *outbuf[4];
	int outlinesize[4] = { out_width, out_width / 2, out_width / 2, 0 };
	outbuf[0] = (uint8_t *)malloc(out_width*out_height);
	outbuf[1] = (uint8_t *)malloc(out_width*out_height >> 2);
	outbuf[2] = (uint8_t *)malloc(out_width*out_height >> 2);
	outbuf[3] = NULL;

	struct SwsContext *img_convert_ctx = nullptr;
	img_convert_ctx = sws_getContext(in_width, in_height, AV_PIX_FMT_RGB24,
		out_width, out_height, AV_PIX_FMT_YUV420P, SWS_POINT, nullptr, nullptr, nullptr);
	if (img_convert_ctx == NULL) {
		fprintf(stderr, "Cannot initialize the conversion context!\n");
		return -1;
	}

	int32_t in_size = in_width * in_height * 3;
	int32_t out_y_size;

	uint8_t * inData[1];
	inData[0] = inbuf; 
	int inLinesize[1] = { 3 * in_width};

	bool bExit = false;
	while (!bExit) {
		if ((fread(ptr_src_rgb_buf, 1, read_size, fin) < 0) || (feof(fin))) {
			bExit = true;
			break;
		}

		memcpy(inbuf, ptr_src_rgb_buf, in_size);
		sws_scale(img_convert_ctx, inData, inLinesize, 0, in_height, outbuf, outlinesize);

		memcpy(ptr_dst_yuv_buf, outbuf[0], out_width*out_height);
		memcpy(ptr_dst_yuv_buf + out_width * out_height, outbuf[1], out_width*out_height >> 2);
		memcpy(ptr_dst_yuv_buf + (out_width*out_height * 5 >> 2), outbuf[2], out_width*out_height >> 2);

		fwrite(ptr_dst_yuv_buf, 1, write_size, fout);
	}

	std::cout << "hello rgb2yuv end!" << endl;
	sws_freeContext(img_convert_ctx);

	fclose(fin);
	fclose(fout);

	delete[] ptr_src_rgb_buf;
	ptr_src_rgb_buf = nullptr;

	delete[] ptr_dst_yuv_buf;
	ptr_dst_yuv_buf = nullptr;

	return 0;
}