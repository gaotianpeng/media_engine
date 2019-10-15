#include<jni.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "cvinterface.h"
#include "com_opencv_OpenCVJni.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	el::Configurations conf("d:/log_conf.conf");
	// 重新配置一个单一日志记录器
	el::Loggers::reconfigureLogger("default", conf);
	// 用配置文件配置所有的日志记录器
	el::Loggers::reconfigureAllLoggers(conf);
	// 所有的日志记录器都是从文件中配置完成的，可以使用
	return JNI_OK;
}


JNIEXPORT jstring JNICALL Java_com_opencv_OpenCVJni_stringFromJNI
  (JNIEnv *env, jclass) {
        std::string hello = "Hello from C++1111";
		LOG(INFO) << "My first info log using default logger";
		return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL Java_com_opencv_OpenCVJni_initHistParam
  (JNIEnv *env, jobject instance, jint hbin, jint sbin, jfloatArray hranges, jfloatArray sranges) {

	CVInterface* pimple = CVInterface::getInstance();
	if (!pimple) {
		//LOG("Failed to get cvinterface!!!!");
		return;
	}

	jfloat tmpArr[2];

	HistFeatureParam histparam;
	histparam.hbin = hbin;
	histparam.sbin = sbin;
	env->GetFloatArrayRegion(hranges, 0, 2, tmpArr);
	//LOG("hranges %f, %f ", tmpArr[0], tmpArr[1]);
	histparam.hranges[0] = tmpArr[0];
	histparam.hranges[1] = tmpArr[1];
	env->GetFloatArrayRegion(sranges, 0, 2, tmpArr);
	//LOG("hranges %f, %f ", tmpArr[0], tmpArr[1]);
	histparam.sranges[0] = tmpArr[0];
	histparam.sranges[1] = tmpArr[1];
}

JNIEXPORT void JNICALL Java_com_opencv_OpenCVJni_getHistFeatureFromImage
  (JNIEnv *env, jobject instance, jstring imgPath, jfloatArray feature, jint arrSize) {

	jboolean isCopy = false;
	const char* path = env->GetStringUTFChars(imgPath, &isCopy);
	if (!path) {
		//LOG("Invalid image path!!!!");
		return;
	}

	CVInterface* pimple = CVInterface::getInstance();
	if (!pimple) {
		//LOG("Failed to cvinterface!!!!");
		return;
	}

	jfloat* tmpFeature = env->GetFloatArrayElements(feature, &isCopy);

	pimple->GetHistFeature(path, static_cast<float*>(tmpFeature), arrSize);

	env->ReleaseStringUTFChars(imgPath, path);
	env->ReleaseFloatArrayElements(feature, tmpFeature, JNI_COMMIT);
}

JNIEXPORT jboolean JNICALL Java_com_opencv_OpenCVJni_compareHistFeature
  (JNIEnv *env, jobject instance, jfloatArray baseHistFeature, jfloatArray targetHistFeature, jint arrSize, jdouble threshold, jint methodType) {

	CVInterface* pimple = CVInterface::getInstance();
	if (!pimple) {
		//LOG("Failed to get cvinterface!!!!");
		return false;
	}

	jboolean isCopy = false;
	jfloat* tmpBase = env->GetFloatArrayElements(baseHistFeature, &isCopy);
	jfloat* tmpTarget = env->GetFloatArrayElements(targetHistFeature, &isCopy);
	if (!tmpBase || !tmpTarget) {
		//LOG("Invalid hash codedata !!!!");
		return false;
	}
	bool bRet = pimple->CompareHistFeature(static_cast<float*>(tmpBase), static_cast<float*>(tmpTarget),
		arrSize, threshold, methodType);

	env->ReleaseFloatArrayElements(baseHistFeature, tmpBase, JNI_COMMIT);
	env->ReleaseFloatArrayElements(targetHistFeature, tmpTarget, JNI_COMMIT);

	return bRet;
    return false;
}

JNIEXPORT void JNICALL Java_com_opencv_OpenCVJni_getHashCodeFromImageByte
  (JNIEnv *env, jobject instance, jstring imgPath, jbooleanArray hashcode, jint arrSize, jint methodType) {

	CVInterface* pimple = CVInterface::getInstance();
	if (!pimple) {
		//LOG("Failed to get cvinterface!!!!");
		return;
	}

	jboolean isCopy = false;
	const char* path = env->GetStringUTFChars(imgPath, &isCopy);
	if (!path) {
		//LOG("Invalid image path!!!!");
		return;
	}

	jboolean* tmpBase = env->GetBooleanArrayElements(hashcode, &isCopy);
	if (!tmpBase) {
		//LOG("Invalid hash codedata !!!!");
		return;
	}

	pimple->GetHashCode(path, static_cast<uchar*>(tmpBase), arrSize, methodType);

	env->ReleaseStringUTFChars(imgPath, path);
	env->ReleaseBooleanArrayElements(hashcode, tmpBase, JNI_COMMIT);
}

JNIEXPORT void JNICALL Java_com_opencv_OpenCVJni_getHashCodeFromImageDouble
  (JNIEnv *env, jobject instance, jstring imgPath, jdoubleArray hashCode, jint arrSize, jint methodType) {

	CVInterface* pimple = CVInterface::getInstance();
	if (!pimple) {
		//LOG("Failed to get cvinterface!!!!");
		return;
	}

	jboolean isCopy = false;
	const char* path = env->GetStringUTFChars(imgPath, &isCopy);
	if (!path) {
		//LOG("Invalid image path!!!!");
		return;
	}

	jdouble* hashCodeTmp = env->GetDoubleArrayElements(hashCode, &isCopy);
	if (!hashCodeTmp) {
		//LOG("Invalid hash codedata !!!!");
		return;
	}

	pimple->GetHashCode(path, static_cast<double*>(hashCodeTmp), arrSize, methodType);

	env->ReleaseStringUTFChars(imgPath, path);
	env->ReleaseDoubleArrayElements(hashCode, hashCodeTmp, JNI_COMMIT);
}

JNIEXPORT jboolean JNICALL Java_com_opencv_OpenCVJni_compareHashCodeByte
  (JNIEnv *env, jobject instance, jbooleanArray baseHashcode, jbooleanArray targetHashcode, jint arrSize, jdouble threshold, jint methodType) {

	CVInterface* pimple = CVInterface::getInstance();
	if (!pimple) {
		//LOG("Failed to get cvinterface!!!!");
		return false;
	}

	jboolean isCopy = false;
	jboolean* tmpBase = env->GetBooleanArrayElements(baseHashcode, &isCopy);
	jboolean* tmpTarget = env->GetBooleanArrayElements(targetHashcode, &isCopy);
	if (!tmpBase || !tmpTarget) {
		//LOG("Invalid hist feature data !!!!");
		return false;
	}

	bool bRet = pimple->CompareHashCode(static_cast<uchar*>(tmpBase), static_cast<uchar*>(tmpTarget),
		arrSize, threshold, methodType);
	env->ReleaseBooleanArrayElements(baseHashcode, tmpBase, JNI_COMMIT);
	env->ReleaseBooleanArrayElements(targetHashcode, tmpTarget, JNI_COMMIT);

	return bRet;
}

JNIEXPORT jboolean JNICALL Java_com_opencv_OpenCVJni_compareHashCodeDouble
  (JNIEnv *env, jobject instance, jdoubleArray baseHashcode, jdoubleArray targetHashcode, jint arrSize, jdouble threshold, jint methodType) {

	CVInterface* pimple = CVInterface::getInstance();
	if (!pimple) {
		//LOG("Failed to get cvinterface!!!!");
		return false;
	}

	jboolean isCopy = false;
	jdouble* tmpBase = env->GetDoubleArrayElements(baseHashcode, &isCopy);
	jdouble* tmpTarget = env->GetDoubleArrayElements(targetHashcode, &isCopy);
	if (!tmpBase || !tmpTarget) {
		//LOG("Invalid hist feature data !!!!");
		return false;
	}

	bool bRet = pimple->CompareHashCode(static_cast<double*>(tmpBase), static_cast<double*>(tmpTarget),
		arrSize, threshold, methodType);

	env->ReleaseDoubleArrayElements(baseHashcode, tmpBase, JNI_COMMIT);
	env->ReleaseDoubleArrayElements(targetHashcode, tmpTarget, JNI_COMMIT);
	return bRet;
}

JNIEXPORT void JNICALL Java_com_opencv_OpenCVJni_printHistFeature
  (JNIEnv *env, jobject instance, jfloatArray feature, jint arrSize) {

	jboolean isCopy = false;
	jfloat* featureBuf = env->GetFloatArrayElements(feature, &isCopy);
	if (!featureBuf) {
		//LOG("Invalid hist feature array");
		return;
	}
	//LOG("hist feature is:");
	float* p = featureBuf;
	for (int i = 0; i < arrSize; i++) {
		//LOG("%f, ", *p);
		p++;
	}
	//LOG("\n");
	env->ReleaseFloatArrayElements(feature, featureBuf, JNI_COMMIT);
}

JNIEXPORT void JNICALL Java_com_opencv_OpenCVJni_printHashCodeBype
  (JNIEnv *env, jobject instance, jbooleanArray hashcode, jint arrSize) {

	jboolean isCopy = false;
	jboolean* hashcodeBuf = env->GetBooleanArrayElements(hashcode, &isCopy);
	if (!hashcodeBuf) {
		//LOG("Invalid hashcode array");
		return;
	}
	//LOG("color moment hash code is:");
	unsigned char* p = static_cast<unsigned char*>(hashcodeBuf);
	short temVal = 0;
	for (int i = 0; i < arrSize; i++) {
		temVal = *p;
		//LOG("%d, ", temVal);
		p++;
	}
	//LOG("\n");

	env->ReleaseBooleanArrayElements(hashcode, hashcodeBuf, JNI_COMMIT);
}

JNIEXPORT void JNICALL Java_com_opencv_OpenCVJni_printHashCodeDouble
  (JNIEnv *env, jobject instance, jdoubleArray hashcode, jint arrSize) {

	jboolean isCopy = false;
	jdouble* hashcodeBuf = env->GetDoubleArrayElements(hashcode, &isCopy);
	if (!hashcodeBuf) {
		//LOG("Invalid hashcode array");
		return;
	}
	//LOG("hash code is:");
	double*p = hashcodeBuf;
	for (int i = 0; i < arrSize; i++) {
		//LOG("%lf, ", *p);
		p++;
	}
	//LOG("\n");

	env->ReleaseDoubleArrayElements(hashcode, hashcodeBuf, JNI_COMMIT);
}