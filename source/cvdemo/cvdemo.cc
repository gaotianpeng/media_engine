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
	Mat out;

	for (int i = 0; i < 10000; i++) {
		cvtColor(img, out, COLOR_BGR2YUV);
		cout << "-------------------------------------" << i << endl; 
	}
	cout << "Hello end \n" << endl;
	return 0; 
}

int main1(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, keys);
	Mat src = imread(parser.get<String>("@image_base"), IMREAD_COLOR);
	if (src.empty())
	{
		return -1;
	}
	UMat usrc; 
	src.copyTo(usrc); 

	TickMeter ticker; 
	ticker.reset(); 
	ticker.start(); 
	vector<UMat> ubgr_planes;
	split(usrc, ubgr_planes);
	ticker.stop(); 
	cout << "split UMat costs: " << ticker.getTimeMilli() << " ms" << endl;
	//int histSize = 256;

	//float range[] = { 0, 256 };
	//const float* histRange = { range };

	//bool uniform = true, accumulate = false;
	//TickMeter ticker;
	//ticker.reset(); ticker.start();

	//Mat b_hist, g_hist, r_hist;
	//calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	//calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	//calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	//int hist_w = 512, hist_h = 400;
	//int bin_w = cvRound((double)hist_w / histSize);

	//Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	//normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//ticker.stop();
	//cout << "calcHist Mat costs: " << ticker.getTimeMilli() << " ms" << endl;

	//for (int i = 1; i < histSize; i++)
	//{
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
	//		Scalar(255, 0, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
	//		Scalar(0, 255, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
	//		Scalar(0, 0, 255), 2, 8, 0);
	//}

	imshow("Source image", src);
	//imshow("calcHist Demo", histImage);
	waitKey();

	return 0;
}

int main11(int argc, char** argv)
{
	std::cout << CV_VERSION << endl; 
	CommandLineParser parser(argc, argv, keys);
	Mat src = imread(parser.get<String>("@image_base"), IMREAD_COLOR);
	if (src.empty())
	{
		return -1;
	}
	TickMeter ticker;
	ticker.reset(); ticker.start();
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	ticker.stop();
	cout << "split Mat costs: " << ticker.getTimeMilli() << " ms" << endl;

	//int histSize = 256;

	//float range[] = { 0, 256 };
	//const float* histRange = { range };

	//bool uniform = true, accumulate = false;


	//Mat b_hist, g_hist, r_hist;
	//calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	//calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	//calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	//int hist_w = 512, hist_h = 400;
	//int bin_w = cvRound((double)hist_w / histSize);

	//Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	//normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	//for (int i = 1; i < histSize; i++)
	//{
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
	//		Scalar(255, 0, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
	//		Scalar(0, 255, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
	//		Scalar(0, 0, 255), 2, 8, 0);
	//}

	//imshow("Source image", src);
	//imshow("calcHist Demo", histImage);
	//waitKey();

	return 0;
}