#include "Windows.h"
namespace Timing {
	LARGE_INTEGER g_LastFrameStartTick;
	bool isFirstFrame = true;
	
	float CalcLastFrameTime_ms() {
		float	g_LastFrameTime_ms;
		LARGE_INTEGER currentTick;
		LARGE_INTEGER Frequency;
		QueryPerformanceCounter(&currentTick);
		if (!isFirstFrame) {
			// How many ticks have elapsed since we last did this
			long elapsedTicks = (long)(currentTick.QuadPart - g_LastFrameStartTick.QuadPart);

			// Calculate the frame time - converting ticks to ms.
			QueryPerformanceFrequency(&Frequency);
			g_LastFrameTime_ms = (float)elapsedTicks / Frequency.QuadPart * 1000.0f;
		}
		else {
			g_LastFrameTime_ms = 13.3f; // Assume a 60Hz frame for first call.
			isFirstFrame = false;
		}
		g_LastFrameStartTick = currentTick;
		return g_LastFrameTime_ms;
	}
}