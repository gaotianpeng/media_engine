/*
	Copyright:
*/

#pragma  once 

#include "me_utility.h"
#include <windows.h>
#include <stdint.h>

UTILITY_FUNC_API void GetCurProcessMemInfo(uint64_t *mem, uint64_t *vmem);