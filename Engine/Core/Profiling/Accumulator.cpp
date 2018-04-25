#include "EngineLow\Timing\Timing.h"
#include "Accumulator.h"
#include "Profiler.h"
#include <limits>
Accumulator::Accumulator(int i_index):
	m_Sum(0),
	m_Count(0),
	m_Max((std::numeric_limits<long>::min)()),
	m_Min((std::numeric_limits<long>::max)())
{
	//i_pProfiler.registerAccumulator(i_pName, this);
	g_Profiler.registerAccumulator(i_index, this);
}

void Accumulator::operator+=(long i_ticks) {
	m_Sum += i_ticks;
	m_Count++;

	if (i_ticks > m_Max) m_Max = i_ticks;
	else if (i_ticks < m_Min) m_Min = i_ticks;
}
float Accumulator::average() {
	return (float)m_Sum / (float)m_Count;
}

float Accumulator::getAverageTime() {
	return Timing::getTimeDiff_ms((long)average());
}