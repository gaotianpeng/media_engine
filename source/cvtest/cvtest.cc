#include "opencv2/opencv.hpp"

#include <iostream>
#include <vector>

using namespace std; 
using namespace cv; 

const char* keys = {
	"{@test_img|e:/data/lena.jpg|}"
};

const int width = 1280; 
const int height = 720; 
const int channels = 3; 
const int psize = 1280 * 720; 
const int memsize = width * height * 3; 
           
int main(int argc, char* argv[]) {
	cout << "Hello World!\n" << endl;
	CommandLineParser parser(argc, argv, keys);
	string in_file = parser.get<string>("@test_img");
	Mat img = imread(in_file, IMREAD_UNCHANGED);

	Mat out;
	TickMeter meter;
	meter.reset();
	meter.getTimeTicks();
	cout << "before for cycle :" << meter.getCounter() << endl; 
	for (int i = 0; i < 10; i) {
		//COLOR_RGBA2YUV_I420
		cvtColor(img, out, COLOR_BGR2YUV);
	}
	cout << "after for cycle :" << meter.getCounter() << endl;



	return 0; 
}