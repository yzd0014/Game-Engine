#pragma once
#include "EngineLow\Timing\Timing.h"
#include "Accumulator.h"
#include "Core\Utility\Utility.h"
class ScopedTimer {
public:
	ScopedTimer(Accumulator * i_pAccumulator) :
		m_Start(Timing::getCurrentTick()),
		m_pAccumulator(i_pAccumulator)
	{
	}
	~ScopedTimer() {
		LONGLONG currentTick = Timing::getCurrentTick();
		long elapsedTicks = long(currentTick - m_Start);
		*m_pAccumulator += elapsedTicks;
	}
private:	
	LONGLONG m_Start;
	Accumulator * m_pAccumulator;
};