#include<stdio.h>
#include <iostream>
#include "Core\Core.h"
#include "Core\InputSystem\InputHub.h"

namespace Engine
{
	void * initPointer;
	
	void init() {
		InputHub::Init();
		printf("game engine is initialized.\n");
	}
	void initHeap(size_t i_blockSize, int numOfDescriptors) {
		initPointer = _aligned_malloc(i_blockSize, 4);
		HeapManager::create(initPointer, i_blockSize, numOfDescriptors);
		DEBUG_PRINT_INFO("line #%d: start total free memory %zu", __LINE__, HeapManager::heapManager->getTotalFreeMemory());
		DEBUG_PRINT_INFO("line #%d: total number of descriptors %zu", __LINE__, HeapManager::heapManager->getTotalNumDescriptors());
		//initialize fixed size heap manager
		FixedSizeAllocator * pFSA_8 = static_cast<FixedSizeAllocator *>(HeapManager::heapManager->_alloc(sizeof(FixedSizeAllocator)));
		FixedSizeAllocator::FSAs[0] = new (pFSA_8) FixedSizeAllocator(16, 7 * 1024);

		FixedSizeAllocator * pFSA_16 = static_cast<FixedSizeAllocator *>(HeapManager::heapManager->_alloc(sizeof(FixedSizeAllocator)));
		FixedSizeAllocator::FSAs[1] = new (pFSA_16) FixedSizeAllocator(24, 10 * 1024);


		FixedSizeAllocator * pFSA_32 = static_cast<FixedSizeAllocator *>(HeapManager::heapManager->_alloc(sizeof(FixedSizeAllocator)));
		FixedSizeAllocator::FSAs[2] = new (pFSA_32) FixedSizeAllocator(40, 3 * 1024);

		FixedSizeAllocator * pFSA_64 = static_cast<FixedSizeAllocator *>(HeapManager::heapManager->_alloc(sizeof(FixedSizeAllocator)));
		FixedSizeAllocator::FSAs[3] = new (pFSA_64) FixedSizeAllocator(72, 1 * 1024);
	}
	void shutDownHeap() {
		for (int i = 0; i < 4; i++) {
			delete FixedSizeAllocator::FSAs[i];
			FixedSizeAllocator::FSAs[i] = nullptr;
		}

		HeapManager::heapManager->collect();
		DEBUG_PRINT_INFO("line #%d: start total free memory %zu", __LINE__, HeapManager::heapManager->getTotalFreeMemory());
		DEBUG_PRINT_INFO("line #%d: total number of descriptors %zu", __LINE__, HeapManager::heapManager->getTotalNumDescriptors());
		HeapManager::heapManager->destroy();
		HeapManager::heapManager = nullptr;
		_aligned_free(initPointer);
	}
}
