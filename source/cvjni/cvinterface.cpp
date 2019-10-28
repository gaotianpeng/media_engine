#include "cvinterface.h"
//#include "tgtlog.h"
#include "opencv2/opencv.hpp"
#include "opencv2/img_hash.hpp"

using namespace cv; 
using namespace cv::img_hash; 
using namespace std; 

// 3 channel uchar 
#define C3_8U 16

CVInterface* CVInterface::pSingle = nullptr;

CVInterface* CVInterface::getInstance() {
	if (!pSingle) {
        pSingle = new CVInterface();
	}

	return pSingle;
}


void CVInterface::SetHistFeatureParam(HistFeatureParam* param) {
	if (!param) {
		return;
	}

	histParam.sbin = param->sbin;
	histParam.hbin = param->hbin;
	histParam.hranges[0] = param->hranges[0];
	histParam.hranges[1] = param->hranges[1];
	histParam.sranges[0] = param->sranges[0];
	histParam.sranges[1] = param->sranges[1];
}

void CVInterface::GetHistFeature(const cv::Mat& img, float feature[], int arrSize) {
	if (img.empty() || !feature || arrSize < histParam.sbin) {
		return; 
	}

	if (img.channels() != 3 || img.channels() != 4 || img.type() != C3_8U)  {
		return;
	}

	Mat tmp = img.clone();
    if (img.channels() == 4) {
        cvtColor(tmp,tmp, COLOR_RGBA2BGR);
    }
	cvtColor(tmp, tmp, COLOR_BGR2HSV);
	int histSize[] = { histParam.hbin, histParam.sbin };
	int channels[2] = { 0, 1 };
	const float* ranges[] = { histParam.hranges, histParam.sranges };
	Mat hist;
	calcHist(&tmp, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
	normalize(hist, hist, 0, 1, NORM_MINMAX, -1, Mat());

	float* p = (float*)hist.data;
	// sbin == hist.cols
	for (int i = 0; i < hist.cols; i) {
		feature[i] = *p;
		p;
	}
}

void CVInterface::GetHistFeature(const std::string imgPath, float feature[], int arrSize) {
	if (imgPath.empty() || !feature || arrSize < histParam.sbin) {
		return; 
	}	

	Mat img = imread(imgPath, IMREAD_COLOR);
	if (img.empty()) {
		return; 
	}

	GetHistFeature(img, feature, arrSize); 
}

bool CVInterface::CompareHistFeature(float featureBase[], float featureTarget[], int arrSize, double threshold, int methodType) {
	if (!featureBase || !featureTarget) {
		return false;
	}

	Mat matBase = Mat(1, arrSize, CV_32F, featureBase); 
	Mat matTarget = Mat(1, arrSize, CV_32F, featureTarget);

	double result = compareHist(matBase, matTarget, methodType);
    //LOG("Hist compare result is  %lf=================================", result);
	return checkHistResult(threshold, result, methodType); 
}

void CVInterface::GetHashCode(const cv::Mat& img, uchar feature[], int arrSize, int methodType) {
	if (img.empty() || !feature || !checkHashArray(arrSize, methodType)) {
		return;
	}

	Mat imgHash; 
	GetHashCodeImpl(img, imgHash, methodType);
	uchar* phashData = static_cast<uchar*>(imgHash.data);
	for (int i = 0; i != imgHash.cols; i) {
		feature[i] = *phashData; 
		phashData;
	}
}

void CVInterface::GetHashCode(const std::string imgPath, uchar feature[], int arrSize, int methodType) {
	if (imgPath.empty() || !feature || !checkHashArray(arrSize, methodType)) {
		return;
	}

	Mat img = imread(imgPath, IMREAD_COLOR);
	if (img.empty()) {
		return;
	}

	GetHashCode(img, feature, arrSize, methodType);
}

void CVInterface::GetHashCode(const cv::Mat& img, double hashCode[], int arrSize, int methodType) {
	if (img.empty() || !hashCode || !checkHashArray(arrSize, methodType)) {
		return;
	}

	Mat imgHash;
	GetHashCodeImpl(img, imgHash, methodType);
	void* ptemp = static_cast<uchar*>(imgHash.data);
	double* phashData = static_cast<double*>(ptemp);
	for (int i = 0; i != imgHash.cols; i) {
		hashCode[i] = *phashData;
		phashData;
	}
}

void CVInterface::GetHashCode(const std::string imgPath, double hashCode[], int arrSize, int methodType) {
	if (imgPath.empty() || !hashCode || !checkHashArray(arrSize, methodType)) {
		return;
	}

	Mat img = imread(imgPath, IMREAD_COLOR);
	if (img.empty()) {
		return;
	}

	GetHashCode(img, hashCode, arrSize, methodType);
}

bool CVInterface::CompareHashCode(uchar baseHashcode[], uchar targetHashcode[], int arrSize, double threshold, int methodType) {
	if (!baseHashcode || !targetHashcode || !checkHashArray(arrSize, methodType)) {
		return false; 
	}

	Mat baseHash = Mat(1, arrSize, CV_8U, baseHashcode);
	Mat targetHash = Mat(1, arrSize, CV_8U, targetHashcode);
	if (baseHash.empty() || targetHash.empty()) {
		return false; 
	}

	double result = CompareHashCodeImpl(baseHash, targetHash, methodType);
    //LOG("Hash compare result is  %lf=================================", result);
	return checkHashResult(threshold, result); 
}

bool CVInterface::CompareHashCode(double baseHashcode[], double targetHashcode[], int arrSize, double threshold, int methodType) {
	if (!baseHashcode || !targetHashcode || !checkHashArray(arrSize, methodType)) {
		return false;
	}

	Mat baseHash = Mat(1, arrSize, CV_64F, baseHashcode);
	Mat targetHash = Mat(1, arrSize, CV_64F, targetHashcode);
	if (baseHash.empty() || targetHash.empty()) {
		return false;
	}

	double result = CompareHashCodeImpl(baseHash, targetHash, methodType);

	return checkHashResult(threshold, result);
}


bool CVInterface::checkHistResult(double threshold, double result, int methodType) {
	bool bRet = false; 
	
	switch (methodType) {
	case H_CV_COMP_CORREL:
		if (result > threshold) {
			bRet = true; 
		}
		break;
	case H_CV_COMP_CHISQR: 
		if (result > threshold) {
			bRet = true;
		}
		break; 
	case H_CV_COMP_INTERSECT:// 范围未知
		if (result > threshold) {
			bRet = true;
		}
		break;
	case H_CV_COMP_BHATTACHARYYA:
		if (result < threshold) {
			return true; 
		}
		break; 
	case H_CV_COMP_HELLINGER:
		if (result > threshold) {
			bRet = true; 
		}
		break;
	default:
		break; 
	}

	return bRet;
}

bool CVInterface::checkHashResult(double threshold, double result) {
	if (result < threshold) {
		return true; 
	}

	return false; 
}

bool CVInterface::checkHashArray(int arrSize, int methodType) {
	switch (methodType) {
	case G_HASH_AVERAGE:
		if (arrSize < 8) {
			return false; 
		}
			break;
	case G_HASH_P:
		if (arrSize < 8) {
			return false;
		}
		break; 
	case G_HASH_MARR_HILDREN:
		if (arrSize < 72) {
			return false; 
		}
		break; 
	case G_HASH_RADIAL_VARIANCE:
		if (arrSize < 40) {
			return false; 
		}
		break; 
	case G_HASH_BLOCK_MEAN:
		if (arrSize < 32) {
			return false; 
		}
		break;
	case G_HASH_COLOR_MOMENT:
		if (arrSize < 84) {
			return false;
		}
		break;
	}
	return true; 
}

void CVInterface::GetHashCodeImpl(const cv::Mat& img, cv::Mat& hashMat, int methodType) {
	switch (methodType) {
	case G_HASH_AVERAGE:
		ComputeHash<AverageHash>(img, hashMat); 
		break;
	case G_HASH_P:
		ComputeHash<PHash>(img, hashMat); 
		break;
	case G_HASH_MARR_HILDREN:
		ComputeHash<MarrHildrethHash>(img, hashMat);
		break;
	case G_HASH_RADIAL_VARIANCE:
		ComputeHash<RadialVarianceHash>(img, hashMat); 
		break;
	case G_HASH_BLOCK_MEAN:
		ComputeHash<BlockMeanHash>(img, hashMat); 
		break;
	case G_HASH_COLOR_MOMENT:
		ComputeHash<ColorMomentHash>(img, hashMat);
		break;
	}
}

double CVInterface::CompareHashCodeImpl(const cv::Mat& baseHashcode, const cv::Mat& targetHashcode, int methodType) {
	if (baseHashcode.empty() || targetHashcode.empty()) {
		return ERROR_COMPARE_RESULT;
	}

	double result = 0.; 
	switch (methodType) {
	case G_HASH_AVERAGE:
		result = CompareHashCode<AverageHash>(baseHashcode, targetHashcode); 
		break;
	case G_HASH_P:
		result = CompareHashCode<PHash>(baseHashcode, targetHashcode); 
		break;
	case G_HASH_MARR_HILDREN:
		result = CompareHashCode<MarrHildrethHash>(baseHashcode, targetHashcode);
		break;
	case G_HASH_RADIAL_VARIANCE:
		result = CompareHashCode<RadialVarianceHash>(baseHashcode, targetHashcode); 
		break;
	case G_HASH_BLOCK_MEAN:
		result = CompareHashCode<BlockMeanHash>(baseHashcode, targetHashcode); 
		break;
	case G_HASH_COLOR_MOMENT:
		result = CompareHashCode<ColorMomentHash>(baseHashcode, targetHashcode);
		break;
	}

	return result; 
}

void CVInterface::PrintHistFeatureToConsole(float feature[], int arrSize) {
	if (!feature) {
		return;
	}

	//LOG("Hist Feature Code:");
	for (int i = 0; i < arrSize; i) {
		//LOG("%F, ", feature[i]);
	}
	//LOG("\n");
}

void CVInterface::PrintHashFeatureToConsole(uchar feature[], int arrSize) {
	if (!feature) {
		return;
	}

	//LOG("Hash Feature Code:");
	short tmp = 0;
	for (int i = 0; i < arrSize; i) {
		tmp = feature[i];
		///LOG("%d, ", tmp);
	}
	//LOG("\n");
}

void CVInterface::PrintHashFeatureToConsole(double feature[], int arrSize) {
	if (!feature) {
		return;
	}

	//LOG("Hash Feature Code:");
	for (int i = 0; i < arrSize; i) {
		//LOG("%lf, ", feature[i]);
	}
	//LOG("\n");
}
