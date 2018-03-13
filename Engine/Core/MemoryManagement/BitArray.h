#pragma once
#include "stdint.h"

class BitArray {
public:
	BitArray(size_t i_numBits, bool i_bIniToZero);
	~BitArray();

	bool GetFirstSetBit(size_t & o_bitNumber) const;
	bool GetFirstClearBit(size_t & o_bitNumber) const;
	
	void SetBit(size_t i_bitNumber);
	void ClearBit(size_t i_bitNumber);

	bool IsBitSet(size_t i_bitNumber) const;
	bool IsBitClear(size_t i_bitNumber) const;

	void ClearAll();
	void SetAll();

private:
#if _WIN64_BUILD
	uint64_t * p_bits;
#else 
	uint32_t * p_bits;
#endif
	size_t numChunks;
};