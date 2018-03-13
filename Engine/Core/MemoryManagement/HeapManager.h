#pragma once
#include "BlockDescriptor.h"
#include <mutex> 
class HeapManager {
public:
	static HeapManager *heapManager;
	HeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
	inline static HeapManager * create(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
	inline void * _alloc(size_t i_size);
	void * _alloc(size_t i_size, unsigned int i_alignment);
	bool _free(void * i_ptr);
	inline void collect();
	inline bool contains(void * i_ptr) const;
	inline bool isAllocated(void * i_ptr) const;
	inline void showFreeBlocks() const;
	inline size_t getLargestFreeBlock() const;
	inline size_t getTotalFreeMemory() const;
	inline unsigned int getTotalNumDescriptors() const;
	inline void showOutstandingAllocations() const;
	inline void destroy() const;
private:
	BlockDescriptor unusedHeader;
	BlockDescriptor freeHeader;
	BlockDescriptor outstandingHeader;
	void * heapStarter;
	size_t heapSize;
	std::mutex mtx;

	inline void sortInsert(BlockDescriptor * i_ptr);
};

#include "HeapManager-inl.h"

