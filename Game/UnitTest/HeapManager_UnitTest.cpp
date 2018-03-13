//#include "Source/MemoryManagement/HeapManagerProxy.h"
#include <Windows.h>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <stdio.h>

#include "Core\Utility\Utility.h"
#include "Core\MemoryManagement\HeapManager.h"
#include "Core\MemoryManagement\FixedSizeAllocator.h"

//#define SUPPORTS_ALIGNMENT
//#define SUPPORTS_SHOWFREEBLOCKS
#define SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
//#define TEST_SINGLE_LARGE_ALLOCATION
bool HeapManager_UnitTest()
{
	//using namespace HeapManagerProxy;
	const size_t 		sizeHeap = 1024 * 1024;
	const unsigned int 	numDescriptors = 2048;

	// Allocate memory for my test heap.
	void * pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
	assert(pHeapMemory);

	// Create a heap manager for my test heap.
	//HeapManager * pHeapManager = CreateHeapManager(pHeapMemory, sizeHeap, numDescriptors);
	HeapManager::create(pHeapMemory, sizeHeap, numDescriptors);
	
	//assert(pHeapManager);

	DEBUG_PRINT_INFO("line #%d: start total free memory %zu", __LINE__, HeapManager::heapManager->getTotalFreeMemory());
	DEBUG_PRINT_INFO("line #%d: total number of descriptors %zu", __LINE__, HeapManager::heapManager->getTotalNumDescriptors());

	if (HeapManager::heapManager == nullptr)
		return false;

	//initialize 4 fixed size allocators
	FixedSizeAllocator * pFSA_8 = static_cast<FixedSizeAllocator *>(HeapManager::heapManager->_alloc(sizeof(FixedSizeAllocator)));
	FixedSizeAllocator::FSAs[0] = new (pFSA_8) FixedSizeAllocator(16, 11 * 1024);

	FixedSizeAllocator * pFSA_16 = static_cast<FixedSizeAllocator *>(HeapManager::heapManager->_alloc(sizeof(FixedSizeAllocator)));
	FixedSizeAllocator::FSAs[1] = new (pFSA_16) FixedSizeAllocator(24, 10 * 1024);


	FixedSizeAllocator * pFSA_32 = static_cast<FixedSizeAllocator *>(HeapManager::heapManager->_alloc(sizeof(FixedSizeAllocator)));
	FixedSizeAllocator::FSAs[2] = new (pFSA_32) FixedSizeAllocator(40, 3 * 1024);

	FixedSizeAllocator * pFSA_64 = static_cast<FixedSizeAllocator *>(HeapManager::heapManager->_alloc(sizeof(FixedSizeAllocator)));
	FixedSizeAllocator::FSAs[3] = new (pFSA_64) FixedSizeAllocator(72, 1 * 1024);
	{

#ifdef TEST_SINGLE_LARGE_ALLOCATION
		// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
		// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
		{
#ifdef SUPPORTS_SHOWFREEBLOCKS
			ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS

			size_t largestBeforeAlloc = GetLargestFreeBlock(pHeapManager);
			void * pPtr = alloc(pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave);

			if (pPtr)
			{
#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
				printf("After large allocation:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
				ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS
#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
				ShowOutstandingAllocations(pHeapManager);
#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
				printf("\n");
#endif

				size_t largestAfterAlloc = GetLargestFreeBlock(pHeapManager);
				bool success = Contains(pHeapManager, pPtr) && IsAllocated(pHeapManager, pPtr);
				assert(success);

				success = free(pHeapManager, pPtr);
				assert(success);

				Collect(pHeapManager);

#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
				printf("After freeing allocation and garbage collection:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
				ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS
#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
				ShowOutstandingAllocations(pHeapManager);
#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
				printf("\n");
#endif

				size_t largestAfterCollect = GetLargestFreeBlock(pHeapManager);
			}
		}
#endif

		std::vector<void *> AllocatedAddresses;
		AllocatedAddresses.reserve(350);
		long	numAllocs = 0;
		long	numFrees = 0;
		long	numCollects = 0;

		// allocate memory of random sizes up to 1024 bytes from the heap manager
		// until it runs out of memory
		do
		{
			if (numAllocs > 350) break;
			const size_t		maxTestAllocationSize = 1024;

			size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

#ifdef SUPPORTS_ALIGNMENT
			// pick an alignment
			const unsigned int	alignments[] = { 4, 8, 16, 32, 64 };

			const unsigned int	index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

			const unsigned int	alignment = alignments[index];

			void * pPtr = alloc(pHeapManager, sizeAlloc, alignment);


			// check that the returned address has the requested alignment
			assert((reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0);
#else
			//void * pPtr = alloc(pHeapManager, sizeAlloc);
			char * pPtr = new char[sizeAlloc];
#endif // SUPPORT_ALIGNMENT

			// if allocation failed see if garbage collecting will create a large enough block
			if (pPtr == nullptr)
			{
				//Collect(pHeapManager);
				HeapManager::heapManager->collect();

#ifdef SUPPORTS_ALIGNMENT
				pPtr = alloc(pHeapManager, sizeAlloc, alignment);
#else
				//pPtr = alloc(pHeapManager, sizeAlloc);
				pPtr = new char[sizeAlloc];
#endif // SUPPORT_ALIGNMENT

				// if not we're done. go on to cleanup phase of test
				if (pPtr == nullptr)
					break;
			}
			
			assert(pPtr);
			AllocatedAddresses.push_back(pPtr);
			numAllocs++;

			// randomly free and/or garbage collect during allocation phase
			const unsigned int freeAboutEvery = 10;
			const unsigned int garbageCollectAboutEvery = 40;

			if (!AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0))
			{
				void * pPtr = AllocatedAddresses.back();
				AllocatedAddresses.pop_back();

				//bool success = Contains(pHeapManager, pPtr) && IsAllocated(pHeapManager, pPtr);
				bool success = HeapManager::heapManager->contains(pPtr) && HeapManager::heapManager->isAllocated(pPtr);

				assert(success);

				//success = free(pHeapManager, pPtr);
				delete[] pPtr;
				//assert(success);

				numFrees++;
			}

			if ((rand() % garbageCollectAboutEvery) == 0)
			{
				//Collect(pHeapManager);
				HeapManager::heapManager->collect();
				numCollects++;
			}

		} while (1);

#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
		printf("After exhausting allocations:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
		ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS
#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
		//ShowOutstandingAllocations(pHeapManager);
		HeapManager::heapManager->showOutstandingAllocations();

#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
		printf("\n");
#endif

		// now free those blocks in a random order
		if (!AllocatedAddresses.empty())
		{
			// randomize the addresses
			std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());

			// return them back to the heap manager
			while (!AllocatedAddresses.empty())
			{
				void * pPtr = AllocatedAddresses.back();
				AllocatedAddresses.pop_back();

				//bool success = Contains(pHeapManager, pPtr) && IsAllocated(pHeapManager, pPtr);
				bool success = HeapManager::heapManager->contains(pPtr) && HeapManager::heapManager->isAllocated(pPtr);
				assert(success);

				//success = free(pHeapManager, pPtr);
				delete[] pPtr;

				//assert(success);
			}

#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
			printf("After freeing allocations:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
			ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS

#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
			//ShowOutstandingAllocations(pHeapManager);
			HeapManager::heapManager->showOutstandingAllocations();
#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
			printf("\n");
#endif

			// do garbage collection
			//Collect(pHeapManager);
			HeapManager::heapManager->collect();
			// our heap should be one single block, all the memory it started with

#if defined(SUPPORTS_SHOWFREEBLOCKS) || defined(SUPPORTS_SHOWOUTSTANDINGALLOCATIONS)
            printf("After garbage collection:\n");
#ifdef SUPPORTS_SHOWFREEBLOCKS
			ShowFreeBlocks(pHeapManager);
#endif // SUPPORTS_SHOWFREEBLOCKS

#ifdef SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
			//ShowOutstandingAllocations(pHeapManager);
			HeapManager::heapManager->showOutstandingAllocations();

#endif // SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
			printf("\n");
#endif

			// do a large test allocation to see if garbage collection worked
			//void * pPtr = alloc(pHeapManager, sizeHeap / 2);
			/*
			char * pPtr = new char[sizeHeap / 2];
			assert(pPtr);

			if (pPtr)
			{
				//bool success = Contains(pHeapManager, pPtr) && IsAllocated(pHeapManager, pPtr);
				bool success = HeapManager::heapManager->contains(pPtr) && HeapManager::heapManager->isAllocated(pPtr);
				assert(success);

				//success = free(pHeapManager, pPtr);
				delete[] pPtr;
				//assert(success);

			}
			*/
		}
	}
	for (int i = 0; i < 4; i++) {
		delete FixedSizeAllocator::FSAs[i];
		FixedSizeAllocator::FSAs[i] = nullptr;
	}
	HeapManager::heapManager->collect();
	DEBUG_PRINT_INFO("line #%d: start total free memory %zu", __LINE__, HeapManager::heapManager->getTotalFreeMemory());
	DEBUG_PRINT_INFO("line #%d: total number of descriptors %zu", __LINE__, HeapManager::heapManager->getTotalNumDescriptors());
	HeapManager::heapManager->destroy();
	HeapManager::heapManager = nullptr;

	HeapFree(GetProcessHeap(), 0, pHeapMemory);

	// we succeeded
	return true;
}

