#pragma once 

#ifdef MATH_EXPORTS
	#define MATH_FUNC_API __declspec(dllexport)
#else
	#define MATH_FUNC_API __declspec(dllimport)
#endif

MATH_FUNC_API int print_test();