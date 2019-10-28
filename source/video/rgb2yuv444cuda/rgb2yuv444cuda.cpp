#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv; 

const char *src_file_name = "e:/ffmpeg/720p.rgb";
const char *dst_file_name = "e:/ffmpeg/720yuv444cuda.yuv";
const char* keys = {
	"{@test_img|e:/data/lena.jpg|}"
};

const int channels = 3; 
const int width = 1280; 
const int height = 720; 
const int readSize = width * height * channels; 
const int writeSize = readSize;
const int ysize = width * height; 

int main(int argc, char* argv[]) {
	cout << "rgb 2 yuv cuda starting ...." << endl; 
	cout << "raw info:" << endl; 
	cout << "	" << "width:" << width << ",height:" << height << ",channels:" << channels << endl;
	cout << "	readSize: " << readSize << endl; 
	cout << "	writeSize: " << writeSize << endl;

	FILE* file_in = fopen(src_file_name, "rb");
	FILE* file_out = fopen(dst_file_name, "wb");
	if (!file_in || !file_out) {
		cout << "Invalid file!!!!" << endl; 
		return -1; 
	}

	uint8_t* ptr_src_rgb_buf = new uint8_t[readSize]; 
	uint8_t* ptr_dst_yuv444_buf = new uint8_t[writeSize]; 
	if (!ptr_src_rgb_buf || !ptr_dst_yuv444_buf) {
		cout << "Failed to create rgb/yuv buf!!!" << endl; 
	}

	Mat in_img(height, width, CV_8UC3,ptr_src_rgb_buf);
	if (in_img.empty()) {
		cout << "Failed to create in_img Mat!!!" << endl; 
	}
	Mat out_img; 
	bool bExit = false; 
	int i = 0; 
	Mat mat_split[3]; 
	
	while (!bExit) {
		if ((fread(ptr_src_rgb_buf, 1, readSize, file_in) < 0) || feof(file_in)) {
			bExit = true; 
			break; 
		}
		//cvtColor(in_img, out_img, COLOR_RGB2YUV_I420);
		cvtColor(in_img, out_img, COLOR_RGB2YUV);
		split(out_img, mat_split);

		cout << &out_img.data << "-----------------------------------------" <<endl; 

		memcpy(ptr_dst_yuv444_buf, mat_split[0].data, ysize);
		memcpy(ptr_dst_yuv444_buf  ysize, mat_split[1].data, ysize);
		memcpy(ptr_dst_yuv444_buf  ysize  ysize, mat_split[2].data, ysize);
		fwrite(ptr_dst_yuv444_buf, 1, writeSize, file_out); 
		i; 
		cout << "frame " << i << endl;
	}

	delete[] ptr_dst_yuv444_buf; 
	delete[] ptr_src_rgb_buf; 
	fclose(file_in);
	fclose(file_out);
	return 0;
}