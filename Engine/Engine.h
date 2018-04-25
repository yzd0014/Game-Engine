#pragma once

#include "EngineLow\EngineLow.h"
#include "Core\Core.h"
#include "EngineHigh\EngineHigh.h"
#include "GameCommon\GameCommon.h"


namespace Engine
{
	extern void * initPointer;
	extern float deltaTime;
	void init();
	void initHeap(size_t i_blockSize, int numOfDescriptors);
	void shutDownHeap();
	float getDeltaTime();
}