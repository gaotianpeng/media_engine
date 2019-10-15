#pragma once

#include "opencv2/opencv.hpp"
#include <string>

class VideoTool {
public:
	static void StreamToImages(std::string url, 
								std::string outpath, 
								std::string outprefix, 
								int delayms = 40);
};

