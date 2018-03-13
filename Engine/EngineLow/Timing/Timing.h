#pragma once
#include "Windows.h"
namespace Timing {
	extern LARGE_INTEGER g_LastFrameStartTick;
	extern bool isFirstFrame;
	float CalcLastFrameTime_ms();
}