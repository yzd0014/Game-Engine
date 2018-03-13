#pragma once
#include <assert.h>
#include "PtrCounter.h"

template<class T>
class WeakPtr;

template<class T>
class SmartPtr {
	friend class WeakPtr<T>;
public:
	SmartPtr() :
		m_ptr(nullptr),
		m_pRefCounter(nullptr)
	{//defualt constructor

	}
	SmartPtr(T * i_ptr) ://constructor
		m_ptr(i_ptr)
	{
		if (i_ptr == nullptr) {
			m_pRefCounter = nullptr;
		}
		else {
			m_pRefCounter = new PtrCounter(1, 0);
		}
	}
	SmartPtr(const SmartPtr & i_other)://copy constructor
		m_ptr(i_other.m_ptr),
		m_pRefCounter(i_other.m_pRefCounter)
	{
		if(m_pRefCounter != nullptr)(*m_pRefCounter).refCounter++;
	}
	SmartPtr & operator=(const SmartPtr & i_other) {//asignment operator
		if (m_ptr != i_other.m_ptr) {
			if (m_ptr != nullptr) {
				ReleaseCurrentReference();
			}
			AquireNewReference(i_other);
		}
		return *this;
	}
	SmartPtr & operator=(T * i_ptr) {//assigment operator for null
		if (m_ptr != i_ptr) {
			if (m_ptr != nullptr) {
				ReleaseCurrentReference();
			}

			if (i_ptr == nullptr) {
				m_ptr = i_ptr;
				m_pRefCounter = nullptr;
			}
			else {
				m_ptr = i_ptr;
				m_pRefCounter = new PtrCounter(1, 0);
			}
		}
		return *this;
	}

	bool operator>(const SmartPtr & i_other) {
		return m_ptr > i_other.m_ptr;
	}
	bool operator>(const T * i_ptr) {
		return m_ptr > i_ptr;
	}

	bool operator>=(const SmartPtr & i_other) {
		return m_ptr >= i_other.m_ptr;
	}
	bool operator>=(const T * i_ptr) {
		return m_ptr >= i_ptr;
	}

	bool operator<(const SmartPtr & i_other) {
		return m_ptr < i_other.m_ptr;
	}
	bool operator<(const T * i_ptr) {
		return m_ptr < i_ptr;
	}

	bool operator<=(const SmartPtr & i_other) {
		return m_ptr <= i_other.m_ptr;
	}
	bool operator<=(const T * i_ptr) {
		return m_ptr <= i_ptr;
	}

	bool operator==(const SmartPtr & i_other) {
		return m_ptr == i_other.m_ptr;
	}
	bool operator==(const T * i_ptr) {
		return m_ptr == i_ptr;
	}
	bool operator!=(const SmartPtr & i_other) {
		return m_ptr != i_other.m_ptr;
	}
	bool operator!=(const T * i_ptr) {
		return m_ptr != i_ptr;
	}
	T * operator->() {
		return m_ptr;
	}
	T & operator*() {
		assert(m_ptr != nullptr);
		return *m_ptr;
	}
	
	~SmartPtr() {//destructor
		ReleaseCurrentReference();
	}

private:
	T * m_ptr;
	PtrCounter * m_pRefCounter;
	void ReleaseCurrentReference() {
		if(m_ptr != nullptr)(*m_pRefCounter).refCounter = (*m_pRefCounter).refCounter - 1;
		if (m_pRefCounter != nullptr && (*m_pRefCounter).refCounter == 0) {
			delete m_ptr;
			m_ptr = nullptr;
			if ((*m_pRefCounter).weakCounter == 0) {
				delete m_pRefCounter;
				m_pRefCounter = nullptr;
			}
		}
		
	}
	void AquireNewReference(const SmartPtr & i_other) {
		m_ptr = i_other.m_ptr;
		m_pRefCounter = i_other.m_pRefCounter;
		if (i_other.m_pRefCounter != nullptr)(*i_other.m_pRefCounter).refCounter++;
	}
};