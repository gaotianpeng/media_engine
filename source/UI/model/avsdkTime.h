#pragma once

#include <winsock.h>
#include <time.h>

//ģ��linux�µ�gettimeofday()����
inline int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;

	return (0);
}

//��ȡ���뼶ʱ��(��avsdk�㷨һ��)
inline unsigned getAvsdkTime()
{
	struct timeval recvtv;
	gettimeofday(&recvtv, NULL);
	return (unsigned)(recvtv.tv_sec * 1000 + (unsigned long long)recvtv.tv_usec / 1000);
}

