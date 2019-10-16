#include "opencv2/opencv.hpp"

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

	Mat out;
	for (int i = 0; i < 10000; i++) {
		//COLOR_RGBA2YUV_I420
		cvtColor(img, out, COLOR_BGR2YUV);
		cout << "-------------------------------------" << i << endl; 
	}
	cout << "Hello end \n" << endl;
	return 0; 
}