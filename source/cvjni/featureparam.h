#ifndef _FEATURE_PARAM_H
#define _FEATURE_PARAM_H

struct HistFeatureParam {
	int hbin = 50;
	int sbin = 60;
	float hranges[2] = { 0, 180 };
	float sranges[2] = { 0, 256 };
};

#endif //_FEATURE_PARAM_H