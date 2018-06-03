#include "stdint.h"
#include "assert.h"
#include <intrin.h> 
#include "BitArray.h"
#include "HeapManager.h"
#include "Core\Utility\Utility.h"

BitArray::BitArray(size_t i_numBits, bool i_bIniToZero) {
	assert(HeapManager::heapManager);
#if _WIN64_BUILD
	numChunks = i_numBits / 64;
	p_bits = static_cast<uint64_t *>(HeapManager::heapManager->_alloc(i_numBits / 8));
#else
	numChunks = i_numBits / 32;
	p_bits = static_cast<uint32_t *>(HeapManager::heapManager->_alloc(i_numBits / 8));
#endif
	assert(p_bits);
	memset(p_bits, i_bIniToZero ? 0 : ~0, i_numBits / 8);
}

bool BitArray::GetFirstSetBit(size_t & o_bitNumber) const{
	size_t index = 0;
	while ((p_bits[index] == 0x00) && (index < numChunks))
		index++;
	if (index == numChunks) {
		return false;
	}
	unsigned long bit;
	unsigned char isNonzero;
#if _WIN64_BUILD
	uint64_t mask = p_bits[index];
	isNonzero = _BitScanForward64(&bit, mask);
	o_bitNumber = index * 64 + bit;
#else 
	uint32_t mask = p_bits[index];
	isNonzero = _BitScanForward(&bit, mask);
	o_bitNumber = index * 32 + bit;
#endif
	return true;
}

bool BitArray::GetFirstClearBit(size_t & o_bitNumber) const {
#if _WIN64_BUILD
	uint64_t complement;
#else 
	uint32_t complement;
#endif	
	for (size_t i = 0; i < numChunks; i++) {
		complement = ~(p_bits[i]);
		unsigned long bit;
		unsigned char isNonzero;
		if (complement != 0x00) {
#if _WIN64_BUILD
			isNonzero = _BitScanForward64(&bit, complement);
			o_bitNumber = i * 64 + bit;
#else
			isNonzero = _BitScanForward(&bit, complement);
			o_bitNumber = i * 32 + bit;
#endif
			return true;
		}
	}

	return false;
}

void BitArray::SetBit(size_t i_bitNumber) {
#if _WIN64_BUILD	
	_bittestandset64(reinterpret_cast<long long *>(p_bits), i_bitNumber);
#else 
	_bittestandset(reinterpret_cast<long *>(p_bits), i_bitNumber);
#endif
}

void BitArray::ClearBit(size_t i_bitNumber) {
#if _WIN64_BUILD
	_bittestandreset64(reinterpret_cast<long long *>(p_bits), i_bitNumber);
#else
	_bittestandreset(reinterpret_cast<long *>(p_bits), i_bitNumber);
#endif
}

bool BitArray::IsBitSet(size_t i_bitNumber) const {
	unsigned char bitValue;
#if _WIN64_BUILD
	bitValue = _bittest64(reinterpret_cast<long long *>(p_bits), i_bitNumber);
#else
	bitValue = _bittest(reinterpret_cast<long *>(p_bits), i_bitNumber);
#endif
	if (bitValue == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool BitArray::IsBitClear(size_t i_bitNumber) const {
	unsigned char bitValue;
#if _WIN64_BUILD
	bitValue = _bittest64(reinterpret_cast<long long *>(p_bits), i_bitNumber);
#else
	bitValue = _bittest(reinterpret_cast<long *>(p_bits), i_bitNumber);
#endif
	if (bitValue == 0) {
		return true;
	}
	else {
		return false;
	}
}

void BitArray::ClearAll() {
#if _WIN64_BUILD
	memset(p_bits, 0, numChunks * 8);
#else 
	memset(p_bits, 0, numChunks * 4);
#endif	
}

void BitArray::SetAll() {
#if _WIN64_BUILD
	memset(p_bits, ~0, numChunks * 8);
#else 
	memset(p_bits, ~0, numChunks * 4);
#endif	
}

BitArray::~BitArray() {
	DEBUG_PRINT_INFO("line #%d: BitArray is destroyed sucessfully", __LINE__);
	HeapManager::heapManager->_free(p_bits);
}
