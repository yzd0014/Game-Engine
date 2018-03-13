#pragma once
#include "BitArray.h"
#include <mutex> 
class FixedSizeAllocator {
public:
	static FixedSizeAllocator * FSAs[4];
	FixedSizeAllocator(size_t i_sizeBlock, size_t i_numBlocks);
	//FixedSizeAllocator * create(size_t sizeBlock, size_t numBlocks);
	void * _alloc();
	bool _free(void * i_ptr);
	
	bool contains(void * i_ptr) const;
	
	~FixedSizeAllocator();
private:
	size_t sizeBlock;
	size_t numBlocks;
	BitArray * pAvailableBits; //address to a bit array 
	void * pMStarter;//the first address of the chunk of memeory this allocator has
	std::mutex mtx;
};
