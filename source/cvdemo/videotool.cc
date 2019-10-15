#include "videotool.h"
#include <iostream>

using namespace cv; 
using namespace std; 

void VideoTool::StreamToImages( std::string url, 
								std::string outpath, 
								std::string outprefix,
								int delayms) {
	if (url.empty()) {
		std::cout << "Invalid url!!!" << std::endl; 
		return; 
	}

	VideoCapture cap(url); 
	if (!cap.isOpened()) {
		std::cout << "Failed to open url" << std::endl; 
		return; 
 	}

	Mat frame;
	int imgidx = 0; 
	while (true) {
		cap >> frame; 
		if (frame.empty()) {
			break; 
		}

		if (waitKey(0) == 'c') {
			break; 
		}
		std::stringstream ss; 
		ss << outpath << outprefix <<std::setfill('0') << std::setw(4) << imgidx++ << ".png";
		imwrite(ss.str(), frame);
		std::cout << ss.str() << std::endl; 
		cv::waitKey(delayms); 
	}
}
