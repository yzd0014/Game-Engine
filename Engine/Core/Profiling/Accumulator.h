#pragma once
#include <cstdint>
class Accumulator {
public:
	Accumulator(int i_index);
	void operator+=(long i_ticks);
	float average();
	float getAverageTime();

	long m_Sum;
	long m_Count;
	long m_Min;
	long m_Max;
};