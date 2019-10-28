//
// Created by gtp on 2019/9/20.
//

#include "cvimplement.h"
//#include "tgtlog.h"
#include "cvinterface.h"
#include "featureparam.h"
#include "opencv2/opencv.hpp"
#include "opencv2/img_hash.hpp"

#include <string>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::img_hash;

#define HASH_ARR 32
typedef double HASHCODE_TYPE;


int testHash() {
    string img_dir = "/storage/emulated/0/DCIM/Camera/";
    string a_path = img_dir  "test0001.png";
    string b_path = img_dir  "test0002.png";

    Mat img_a = imread(a_path, IMREAD_COLOR);
    Mat img_b = imread(b_path, IMREAD_COLOR);
    if (img_a.empty() || img_b.empty()) {
        //LOG("invalid image data =================================");
        return -1;
    }

    CVInterface* pimple = CVInterface::getInstance();
    if (!pimple) {
        return -1;
    }

    double hashcodeArrA[HASH_ARR];
    double hashcodeArrB[HASH_ARR];
    int arrSize = HASH_ARR;
//    int methodType = G_HASH_AVERAGE;
   // int methodType = G_HASH_P;
       // int methodType = G_HASH_MARR_HILDREN;
     //int methodType = G_HASH_RADIAL_VARIANCE;
   //int methodType = G_HASH_BLOCK_MEAN;
    // 测试其它种类hash算法 hashcodeArrA hashcodeArrB 均为double型
    int methodType = G_HASH_COLOR_MOMENT;
    double hashThreshold = 28.0;

    TickMeter meter;
    meter.reset();meter.start();
    pimple->GetHashCode(img_a, hashcodeArrA, arrSize, methodType);
    meter.stop();
    //LOG("get hash code from raw consume  %lf ms", meter.getTimeMilli());
    meter.reset();meter.start();
    pimple->GetHashCode(b_path, hashcodeArrB, arrSize, methodType);
    meter.stop();
    //LOG("get hash code from image consume  %lf ms", meter.getTimeMilli());

    meter.reset();meter.start();
    pimple->CompareHashCode(hashcodeArrA, hashcodeArrB, arrSize, hashThreshold, methodType);
    meter.stop();
    //LOG("compare hash code consume  %lf ms", meter.getTimeMilli());

    return 0;
}

int testHashBatch() {
    string img_dir = "/storage/emulated/0/DCIM/Camera/";
    string img_path = img_dir  "test0000.png";

    Mat img = imread(img_path, IMREAD_COLOR);
    if (img.empty()) {
        //LOG("invalid image data =================================");
        return -1;
    }

    CVInterface* pimple = CVInterface::getInstance();
    if (!pimple) {
        return -1;
    }

    uchar hashcodeA[HASH_ARR];
//    double hashcodeA[HASH_ARR];
    int arrSize = HASH_ARR;
    //int methodType = G_HASH_AVERAGE;
     //int methodType = G_HASH_P;
     // int methodType = G_HASH_MARR_HILDREN;
    //int methodType = G_HASH_RADIAL_VARIANCE;
    int methodType = G_HASH_BLOCK_MEAN;
    // 测试其它种类hash算法 hashcodeArrA hashcodeArrB 均为double型
    //int methodType = G_HASH_COLOR_MOMENT;
    double hashThreshold = 28.;
    pimple->GetHashCode(img_path, hashcodeA, arrSize, methodType);

    uchar hashcodeB[HASH_ARR];
    uchar hashcodeC[HASH_ARR];

//    double hashcodeB[HASH_ARR];
//    double hashcodeC[HASH_ARR];
    vector<string> img_names;
    string img_sufix = "*.png";
    glob(img_dir  img_sufix, img_names);
    size_t images = img_names.size();
    TickMeter meter;
    static double totalGetMs = 0;
    static double totalGetMsRaw = 0;
    static double totalComputeMs = 0;
    Mat tmpMat;
    for (size_t i = 0; i != images; i) {
        meter.reset(); meter.start();
        pimple->GetHashCode(img_names[i], hashcodeB, arrSize, methodType);
        meter.stop(); totalGetMs = meter.getTimeMilli();


        tmpMat = imread(img_names[i], IMREAD_COLOR);
        meter.reset(); meter.start();
        pimple->GetHashCode(tmpMat, hashcodeC, arrSize, methodType);
        meter.stop(); totalGetMsRaw = meter.getTimeMilli();

        meter.reset(); meter.start();
        pimple->CompareHashCode(hashcodeA, hashcodeB, arrSize, hashThreshold, methodType);
        meter.stop();
        totalComputeMs = meter.getTimeMilli();
    }
    //LOG("------------------------------G_HASH_BLOCK_MEAN---------------------------------------");
    //LOG("get hash from image batch consume  %lf ms",totalGetMs);
    //LOG("get hash from image average consume  %lf ms", totalGetMs/images);
    //LOG("get hash from raw batch consume  %lf ms", totalGetMsRaw);
    //LOG("get hash from raw average consume  %lf ms", totalGetMsRaw/images);
    //LOG("compare hash batch consume  %lf ms",totalComputeMs);
    //LOG("compare hash average consume  %lf ms", totalComputeMs/images);

    return 0;
}

int testHist() {
    string img_dir = "/storage/emulated/0/DCIM/Camera/";
    string a_path = img_dir  "test0001.png";
    string b_path = img_dir  "test0002.png";

    CVInterface* pimple = CVInterface::getInstance();
    if (!pimple) {
    	return -1;
    }

    Mat img_a = imread(a_path, IMREAD_COLOR);
    Mat img_b = imread(b_path, IMREAD_COLOR);
    if (img_a.empty() || img_b.empty()) {
        //LOG("invalid image data =================================");
        return -1;
    }

    // test case about GetHistFeature
    HistFeatureParam param;
    // arr_size == param.sbin
    float histFeatureA[60] = { 0.f };
    float histFeatureB[60] = { 0.f };
    int histFeatureSize = 60;
    double threshold = 10.;
//    int methodType = H_CV_COMP_CORREL;
    //int methodType = H_CV_COMP_CHISQR;
       // int methodType = H_CV_COMP_INTERSECT;
    //int methodType = H_CV_COMP_BHATTACHARYYA;
    int methodType = H_CV_COMP_HELLINGER;
    pimple->SetHistFeatureParam(&param);
    // get feature from image data
    //pimple->GetHistFeature(a_path, histFeatureA, featureArrSize);
    //pimple->GetHistFeature(b_path, histFeatureB, featureArrSize);
    // get feature from raw data
    TickMeter meter;
    meter.reset(); meter.start();
    pimple->GetHistFeature(img_a, histFeatureA, histFeatureSize);
    meter.stop();
    //LOG("get hist feature from raw consume  %lf ms", meter.getTimeMilli());

    meter.reset(); meter.start();
    pimple->GetHistFeature(b_path, histFeatureB, histFeatureSize);
    meter.stop();
    //LOG("get hist feature from image consume  %lf ms", meter.getTimeMilli());

    // test case about CompareHistFeature
    meter.reset();meter.start();
    pimple->CompareHistFeature(histFeatureA, histFeatureB, histFeatureSize, threshold, methodType);
    meter.stop();
    //LOG("compare hist feature consume  %lf ms", meter.getTimeMilli());
    return 0;
}

int testHistBatch() {
    string img_dir = "/storage/emulated/0/DCIM/Camera/";
    string img_path = "/storage/emulated/0/DCIM/Camera/test0000.png";

    CVInterface* pimple = CVInterface::getInstance();
    if (!pimple) {
        return -1;
    }

    vector<string> img_names;
    string img_sufix = "*.png";
    glob(img_dir  img_sufix, img_names);

    // test case about GetHistFeature
    Mat img = imread(img_path, IMREAD_COLOR);
    HistFeatureParam param;
    // featureArrSize == param.sbin
    int featureArrSize = 60;
    float baseFeatureArr[60] = { 0.f };

    double threshold = 10.;
    //int methodType = H_CV_COMP_CORREL;
    //int methodType = H_CV_COMP_CHISQR;
    //int methodType = H_CV_COMP_INTERSECT;
    //int methodType = H_CV_COMP_BHATTACHARYYA;
    int methodType = H_CV_COMP_HELLINGER;
    pimple->SetHistFeatureParam(&param);

    // get feature from image data
    //pimple->GetHistFeature(img, baseFeatureArr, featureArrSize);
    // get feature from raw data
    pimple->GetHistFeature(img, baseFeatureArr, featureArrSize);

    // test case about CompareHistFeature
    float targetFeatureArr[60];
    float targetFeatureArrC[60];
    int images = img_names.size();

    TickMeter meter;
    static double totalGetMs = 0;
    static double totalGetMsRaw = 0;
    static double totalComputeMs = 0;

    Mat temMat;
    for (int i = 0; i < images; i) {
        meter.reset(); meter.start();
        pimple->GetHistFeature(img_names[i], targetFeatureArr, featureArrSize);
        meter.stop(); totalGetMs = meter.getTimeMilli();

        temMat = imread(img_names[i], IMREAD_COLOR);
        meter.reset(); meter.start();
        pimple->GetHistFeature(temMat, targetFeatureArrC, featureArrSize);
        meter.stop(); totalGetMsRaw = meter.getTimeMilli();

        meter.reset(); meter.start();
        pimple->CompareHistFeature(baseFeatureArr, targetFeatureArr, featureArrSize, threshold, methodType);
        meter.stop();
        totalComputeMs = meter.getTimeMilli();
    }
    //LOG("-----------------------------H_CV_COMP_HELLINGER----------------------------------------");
    //LOG("get hist from image batch consume  %lf ms", totalGetMs);
    //LOG("get hist from image average consume  %lf ms", totalGetMs/images);
    //LOG("get hist from raw batch consume  %lf ms", totalGetMsRaw);
    //LOG("get hist from raw average consume  %lf ms", totalGetMsRaw/images);
    //LOG("compare hist batch consume  %lf ms", totalComputeMs);
    //LOG("compare hist average consume  %lf ms", totalComputeMs/images);
    return 0;
}


