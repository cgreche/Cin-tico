
//OSDTime
// File: time.cpp
// Last Edit: 10/12/2014 03:47 (UTC-3)
// Author: CGR


//WINDOWS Timing functions

#include <windows.h>
#include "..\\..\\types.h"
#include "..\\time.h"

static osd_ticks_t g_tps = 0;


osd_ticks_t osd_ticks()
{
	LARGE_INTEGER freq;
	if(!g_tps) {
		QueryPerformanceFrequency(&freq);
		if(freq.QuadPart == 0)
			return 0;
		g_tps = freq.QuadPart;
	}

	QueryPerformanceCounter(&freq);
	return (s64)freq.QuadPart;
}




osd_ticks_t osd_tps() {
	return g_tps;
}



//Copied from MAME
void osd_sleep(osd_ticks_t ticksToSleep)
{
	DWORD msec;

	// make sure we've computed ticks_per_second
	if(g_tps == 0)
		osd_ticks();

	// convert to milliseconds, rounding down
	msec = (DWORD)(ticksToSleep * 1000 / g_tps);

	// only sleep if at least 2 full milliseconds
	if (msec >= 2)
	{
		HANDLE current_thread = GetCurrentThread();
		int old_priority = GetThreadPriority(current_thread);

		// take a couple of msecs off the top for good measure
		msec -= 2;

		// bump our thread priority super high so that we get
		// priority when we need it
		SetThreadPriority(current_thread, THREAD_PRIORITY_TIME_CRITICAL);
		Sleep(msec);
		SetThreadPriority(current_thread, old_priority);
	}

}