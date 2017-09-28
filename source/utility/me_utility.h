#pragma once 


#ifdef UTILITY_EXPORTS
	#define UTILITY_FUNC_API __declspec(dllexport)
#else
	#define UTILITY_FUNC_API __declspec(dllimport)
#endif
