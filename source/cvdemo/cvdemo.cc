#include "opencv2/opencv.hpp"
#include "videotool.h"

#include <iostream>
#include <vector>

using namespace std; 
using namespace cv; 

const char* keys = {
	"{@test_img|e:/data/lena.jpg|}"
};
           

int main(int argc, char* argv[]) {
	cout << "Hello World!\n" << endl;
	CommandLineParser parser(argc, argv, keys);
	string in_file = parser.get<string>("@test_img");
	Mat img = imread(in_file, IMREAD_UNCHANGED);
	cout << "source image's size: " <<img.size << endl; 
	Mat out;

	cvtColor(img, out, COLOR_BGR2YUV);
	cout <<"--------------------" << out.channels() << endl; 
	cout << "--------------------row:" << out.rows << " cols: " << out.cols << endl; 
	cout << "source image's size: " << out.size << endl;
	cout << "Hello end \n" << endl;

	waitKey(0);
	return 0; 
}
