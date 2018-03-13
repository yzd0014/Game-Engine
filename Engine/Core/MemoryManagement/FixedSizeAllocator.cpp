#include "assert.h"
#include "FixedSizeAllocator.h"
#include "HeapManager.h"
#include "Core\Utility\Utility.h"

FixedSizeAllocator * FixedSizeAllocator::FSAs[4] = { nullptr, nullptr, nullptr, nullptr };

FixedSizeAllocator::FixedSizeAllocator(size_t i_sizeBlock, size_t i_numBlocks): 
	sizeBlock(i_sizeBlock),
	numBlocks(i_numBlocks)
{
	pAvailableBits = static_cast<BitArray *>(HeapManager::heapManager->_alloc(sizeof(BitArray)));
	assert(pAvailableBits);
	pAvailableBits = new (pAvailableBits) BitArray(i_numBlocks, false);
	pMStarter = HeapManager::heapManager->_alloc(i_sizeBlock * i_numBlocks);
	assert(pMStarter);
}

void * FixedSizeAllocator::_alloc() {
	mtx.lock();
	size_t i_firstAvailable;
	if (pAvailableBits->GetFirstSetBit(i_firstAvailable))
	{
		// mark it in use because we're going to allocate it to user
		pAvailableBits->ClearBit(i_firstAvailable);

		// calculate it’s address and return it to user
		uintptr_t output = reinterpret_cast<uintptr_t>(pMStarter) + (i_firstAvailable * sizeBlock) + 4 ;
		mtx.unlock();
		return  reinterpret_cast<void *>(output);
	}
	else {
		mtx.unlock();
		return nullptr;
	}
}

bool FixedSizeAllocator::_free(void * i_ptr) {
	mtx.lock();
	if (contains(i_ptr) == false) {
		return false;
	}
	size_t setBit = (reinterpret_cast<uintptr_t>(i_ptr) - 4 - reinterpret_cast<uintptr_t>(pMStarter)) / sizeBlock;
	pAvailableBits->SetBit(setBit);
	mtx.unlock();
	return true;
}

bool FixedSizeAllocator::contains(void * i_ptr) const {
	if (reinterpret_cast<uintptr_t>(i_ptr) < reinterpret_cast<uintptr_t>(pMStarter) + sizeBlock * numBlocks &&
		reinterpret_cast<uintptr_t>(i_ptr) >= reinterpret_cast<uintptr_t>(pMStarter)) {
		return true;
	}
	else {
		return false;
	}
}

FixedSizeAllocator::~FixedSizeAllocator() {
	size_t firstClear;
	if (pAvailableBits->GetFirstClearBit(firstClear)) {
		DEBUG_PRINT_INFO("line #%d: Unsafe operation, there are still outstanding allocations from fixed size allocator", __LINE__);
	}
	else {
		DEBUG_PRINT_INFO("line #%d: Fixed size allocator is destroyed sucessfully", __LINE__);
	}
	delete pAvailableBits;
	HeapManager::heapManager->_free(pMStarter);
}