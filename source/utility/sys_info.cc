#include "sys_info.h"

#include <stdio.h>
#include <psapi.h>


UTILITY_FUNC_API void GetCurProcessMemInfo(uint64_t *mem, uint64_t *vmem)
{
	PROCESS_MEMORY_COUNTERS pmc;

	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	{
		if (mem)
		{
			*mem = pmc.WorkingSetSize;
		}
		if (vmem)
		{
			*vmem = pmc.PagefileUsage;
		}
	}

}
