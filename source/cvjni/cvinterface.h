#ifndef _CV_INTERFACE_H
#define _CV_INTERFACE_H

#include "opencv2/opencv.hpp"
#include "featureparam.h"
#include "img_hash.hpp"
#include "deftype.h"

/*
 *  本类提供通过直方图及哈希方法提取图片数据特征及对比图片相似度的方法
 *  使用方法参见 cvimplement.cpp
 *  各种方法性能数据参见项目 performance_compare.xlsx
 */
class CVInterface
{
public:
	static CVInterface* getInstance();

	/*
	 * @brief: 设置提取直方图特征参数，需要在调用其它直方图接口之前调用
	 *
	 * @param[in]: param 该参数需要在调用函数之前创建，且已设置默认值
	 *
	 * @param[out]:
	 */
	void SetHistFeatureParam(HistFeatureParam* param);

	/*
	 * @brief: 从bgr图像数据中提取直方图特征
	 *
	 * @param[in]:
	 * 	img: 存放格式为bgr的图片数据
	 * 	arrSize: 存放图片直方图特征数据组的长度
	 *
	 * @param[out]:
	 * 	feature: 存放直方图特征的数组
	 *
 	*/
	void GetHistFeature(const cv::Mat& img, float feature[], int arrSize);

	/*
	 * @brief: 从图片中提取直方图特征
	 *
	 * @param[in]:
	 * 	imgPath: 要提取特征的图片的路径
	 * 	arrSize: 存放图片直方图特征数据组的长度
	 *
	 * @param[out]:
	 * 	feature: 存放直方图特征的数组
	 *
 	*/
	void GetHistFeature(const std::string imgPath, float feature[], int arrSize);

	/*
	 * @brief: 用直方图方法判断两张图片是否相似
	 *
	 *
	 * @param[in]:
	 * 	featureBase: base图像直方图特征数据
	 * 	featureTarget: target图像直方图特征数据
	 * 	arrSize: 图像直方图特征数组长度
	 * 	threshold: 相似度阈值
	 *  methodType: 目前只能是以下值
	 *					H_CV_COMP_CORREL,
	 *					H_CV_COMP_CHISQR,
	 *					H_CV_COMP_INTERSECT,
	 *					H_CV_COMP_BHATTACHARYYA,
	 * 					H_CV_COMP_HELLINGER,
     *
	 * @return:
     *     true: 两张图像相似
     *     false: 两张图像不相似
	 @attention: 具体选用哪种methodType,请参阅项目根目录下algorithm_info.md
	 */
	bool CompareHistFeature(float featureBase[], float featureTarget[], int arrSize, double threshold, int methodType = H_CV_COMP_CORREL);

	/*
	 * @brief: 打印图像直方图特征数据
	 *
	 *
	 * @param[in]:
	 * 	feature: 图像特征数据
	 * 	arrSize: 图像特征数组长度
	 *
	 */
	void PrintHistFeatureToConsole(float feature[], int arrSize);

	/*
	 * @brief: 从bgr图像数据中提取哈希特征
	 *
	 * @param[in]:
	 * 	img: 存放格式为bgr的图片数据
	 * 	arrSize: 存放图片直方图特征数据组的长度
	 *	methodType: 目前只能是以下值
	 *		 			G_HASH_AVERAGE,
	 *          		G_HASH_P,
	 *          		G_HASH_MARR_HILDREN,
	 *          		G_HASH_RADIAL_VARIANCE,
	 *          		G_HASH_BLOCK_MEAN
	 * @param[out]:
	 * 	hashCode: 存放哈希特征的数组
	 *
	 * @attention: methodType与arrSize对应关系如下:
	 * 			G_HASH_AVERAGE  			8
	 * 			G_HASH_P	   				8
	 * 			G_HASH_MARR_HILDREN			72
	 * 			G_HASH_RADIAL_VARIANCE      40
	 *		    G_HASH_BLOCK_MEAN           32
	 *	arrSize越大，哈希算法越准确
	 */
	void GetHashCode(const cv::Mat& img, uchar hashCode[], int arrSize, int methodType = G_HASH_AVERAGE);
	/*
	 * @brief: 从图片数据中提取哈希特征
	 *
	 * @param[in]:
	 * 	imgPath: 存放图片的路径
	 * 	arrSize: 存放图片直方图特征数据组的长度
	 *	methodType: 目前只能是以下值
	 *		 			G_HASH_AVERAGE,
	 *          		G_HASH_P,
	 *          		G_HASH_MARR_HILDREN,
	 *          		G_HASH_RADIAL_VARIANCE,
	 *          		G_HASH_BLOCK_MEAN
	 * @param[out]:
	 * 	hashCode: 存放哈希特征的数组
	 *
	 * @attention: methodType与arrSize对应关系如下:
	 * 			G_HASH_AVERAGE  			8
	 * 			G_HASH_P	   				8
	 * 			G_HASH_MARR_HILDREN			72
	 * 			G_HASH_RADIAL_VARIANCE      40
	 *		    G_HASH_BLOCK_MEAN           32
	 *
	 *	arrSize越大，哈希算法越准确
	 */
	void GetHashCode(const std::string imgPath, uchar hashCode[], int arrSize, int methodType = G_HASH_AVERAGE);
	/*
	 * @brief: 从bgr图像数据中提取哈希特征
	 *
	 * @param[in]:
	 * 	img: 存放格式为bgr的图片数据
	 * 	arrSize: 存放图片直方图特征数据组的长度
	 *	methodType: 目前只能是以下值
	 *				G_HASH_COLOR_MOMENT
	 * @param[out]:
	 * 	hashCode: 存放哈希特征的数组
	 *
	 * @attention: methodType与arrSize对应关系如下:
	 *				G_HASH_COLOR_MOMENT  			84
	 *	arrSize越大，哈希算法越准确
	 *	目前仅支持G_HASH_COLOR_MOMENT
	 */
	void GetHashCode(const cv::Mat& img, double hashCode[], int arrSize, int methodType = G_HASH_COLOR_MOMENT);
	/*
	 * @brief: 从图片数据中提取哈希特征
	 *
	 * @param[in]:
	 * 	imgPath: 存放图片的路径
	 * 	arrSize: 存放图片直方图特征数据组的长度
	 *	methodType: 目前只能是以下值
	 *				G_HASH_COLOR_MOMENT
	 * @param[out]:
	 * 	hashCode: 存放哈希特征的数组
	 *
	 * @attention: methodType与arrSize对应关系如下:
	 * 				G_HASH_COLOR_MOMENT  			84

	 *	arrSize越大，哈希算法越准确
	 *	目前仅支持G_HASH_COLOR_MOMENT
	 */
	void GetHashCode(const std::string imgPath, double hashCode[], int arrSize, int methodType = G_HASH_COLOR_MOMENT);


	/*
	 * @brief: 用哈希方法判断两张图片是否相似
	 *
	 *
	 * @param[in]:
	 * 	baseHashcode: base图像哈希特征数据
	 * 	targetHashcode: target图像哈希特征数据
	 * 	arrSize: 哈希特征数组长度
	 * 	threshold: 相似度阈值
	 *  methodType: 目前只能是以下值
	 *			G_HASH_AVERAGE,
	 *          G_HASH_P,
	 *          G_HASH_MARR_HILDREN,
	 *          G_HASH_RADIAL_VARIANCE,
	 *          G_HASH_BLOCK_MEAN
	 *
	 * @return:
	 *     true: 两张图像相似
	 *     false: 两张图像不相似
	 *
	 * @attention: methodType与arrSize对应关系如下:
	 * 			G_HASH_AVERAGE  			8
	 * 			G_HASH_P	   				8
	 * 			G_HASH_MARR_HILDREN			72
	 * 			G_HASH_RADIAL_VARIANCE      40
	 *		    G_HASH_BLOCK_MEAN           32
	 *		    G_HASH_COLOR_MOMENT			84
	 *	arrSize越大，哈希算法越准确
	 */
	bool CompareHashCode(uchar baseHashcode[], uchar targetHashcode[], int arrSize, double threshold, int methodType = G_HASH_AVERAGE);
	/*
	 * @brief: 用哈希方法判断两张图片是否相似
	 *
	 *
	 * @param[in]:
	 * 	baseHashcode: base图像哈希特征数据
	 * 	targetHashcode: target图像哈希特征数据
	 * 	arrSize: 哈希特征数组长度
	 * 	threshold: 相似度阈值
	 *  methodType: 目前只能是以下值
 *				G_HASH_COLOR_MOMENT
	 *
	 * @return:
	 *     true: 两张图像相似
	 *     false: 两张图像不相似
	 *
	 * @attention: methodType与arrSize对应关系如下:
	 *		    G_HASH_COLOR_MOMENT			84
	 *	arrSize越大，哈希算法越准确
	 */
	bool CompareHashCode(double baseHashcode[], double targetHashcode[], int arrSize, double threshold, int methodType = G_HASH_COLOR_MOMENT);
	/*
	 * @brief: 打印图像哈希特征数据
	 *
	 *
	 * @param[in]:
	 * 	feature: 图像特征数据
	 * 	arrSize: 图像特征数组长度
	 *
	 */
	void PrintHashFeatureToConsole(uchar feature[], int arrSize);
	void PrintHashFeatureToConsole(double feature[], int arrSize);

private:
	bool checkHistResult(double threshold, double result, int methodType);
	bool checkHashResult(double threshold, double result);
	bool checkHashArray(int arrSize, int methodType);

	void GetHashCodeImpl(const cv::Mat& img, cv::Mat& hashMat, int methodType);

	double CompareHashCodeImpl(const cv::Mat& baseHashcode, const cv::Mat& targetHashcode, int methodType);
	template <typename T>
	bool ComputeHash(const cv::Mat &img, cv::Mat& hashMat) {
		if (img.empty()) {
			return false;
		}

		cv::Ptr<cv::img_hash::ImgHashBase> func;
		func = T::create();
		func->compute(img, hashMat);

		return true;
	}

	template <typename T>
	double CompareHashCode(const cv::Mat& baseHashcode, const cv::Mat& targetHashcode) {
		if (baseHashcode.empty() || targetHashcode.empty()) {
			return ERROR_COMPARE_RESULT;
		}

		cv::Ptr<cv::img_hash::ImgHashBase> func;
		func = T::create();
		return func->compare(baseHashcode, targetHashcode);
	}

private:
	HistFeatureParam histParam;
	static CVInterface* pSingle;
};

#endif 