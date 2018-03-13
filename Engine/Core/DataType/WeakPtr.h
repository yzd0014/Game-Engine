#pragma once
#include <assert.h>
#include "PtrCounter.h"
#include "SmartPtr.h"

template<class T>
class WeakPtr {
public:
	WeakPtr() :
		m_ptr(nullptr),
		m_pWeakCounter(nullptr)
	{//defualt constructor

	}
	WeakPtr(T * i_ptr) ://constructor
		m_ptr(i_ptr)
	{
		if (i_ptr == nullptr) {
			m_pWeakCounter = nullptr;
		}
		else {
			m_pWeakCounter = new PtrCounter(0, 1);
		}
	}

	WeakPtr(const SmartPtr<T> & i_other) ://copy constructor
		m_ptr(i_other.m_ptr),
		m_pWeakCounter(i_other.m_pRefCounter)
	{
		if (m_pWeakCounter != nullptr)(*m_pWeakCounter).weakCounter++;
	}
	
	WeakPtr(const WeakPtr & i_other) ://copy constructor
		m_ptr(i_other.m_ptr),
		m_pWeakCounter(i_other.m_pWeakCounter)
	{
		if (m_pWeakCounter != nullptr)(*m_pWeakCounter).weakCounter++;
	}

	WeakPtr & operator=(const SmartPtr<T> & i_other) {//asignment operator
		if (m_ptr != i_other.m_ptr) {
			if (m_ptr != nullptr) {
				ReleaseCurrentReference();
			}
			m_ptr = i_other.m_ptr;
			m_pWeakCounter = i_other.m_pRefCounter;
			if (m_pWeakCounter != nullptr)(*m_pWeakCounter).weakCounter++;
		}
		return *this;
	}

	WeakPtr & operator=(const WeakPtr & i_other) {//asignment operator
		if (m_ptr != i_other.m_ptr) {
			if (m_ptr != nullptr) {
				ReleaseCurrentReference();
			}
			AquireNewReference(i_other);
		}
		return *this;
	}

	WeakPtr & operator=(T * i_ptr) {//assigment operator for null
		if (m_ptr != i_ptr) {
			if (m_ptr != nullptr) {
				ReleaseCurrentReference();
			}

			if (i_ptr == nullptr) {
				m_ptr = i_ptr;
				m_pWeakCounter = nullptr;
			}
			else {
				m_ptr = i_ptr;
				m_pWeakCounter = new PtrCounter(0, 1);
			}
		}
		return *this;
	}

	bool operator>(const WeakPtr & i_other) {
		return m_ptr > i_other.m_ptr;
	}
	bool operator>(const T * i_ptr) {
		return m_ptr > i_ptr;
	}

	bool operator>=(const WeakPtr & i_other) {
		return m_ptr >= i_other.m_ptr;
	}
	bool operator>=(const T * i_ptr) {
		return m_ptr >= i_ptr;
	}

	bool operator<(const WeakPtr & i_other) {
		return m_ptr < i_other.m_ptr;
	}
	bool operator<(const T * i_ptr) {
		return m_ptr < i_ptr;
	}

	bool operator<=(const WeakPtr & i_other) {
		return m_ptr <= i_other.m_ptr;
	}
	bool operator<=(const T * i_ptr) {
		return m_ptr <= i_ptr;
	}

	bool operator==(const WeakPtr & i_other) {
		return m_ptr == i_other.m_ptr;
	}
	bool operator==(const T * i_ptr) {
		return m_ptr == i_ptr;
	}
	bool operator!=(const WeakPtr & i_other) {
		return m_ptr != i_other.m_ptr;
	}
	bool operator!=(const T * i_ptr) {
		return m_ptr != i_ptr;
	}
	/*
	T * operator->() {
		return m_ptr;
	}
	T & operator*() {
		assert(m_ptr != nullptr);
		return *m_ptr;
	}
	*/
	SmartPtr<T> Aquire() {
		if ((*m_pWeakCounter).refCounter == 0) return nullptr;
		SmartPtr<T> o_ptr;
		o_ptr.m_ptr = m_ptr;
		o_ptr.m_pRefCounter = m_pWeakCounter;
		(*o_ptr.m_pRefCounter).refCounter++;
		return o_ptr;
	}
	~WeakPtr() {
		ReleaseCurrentReference();
	}
private:
	T * m_ptr;
	PtrCounter * m_pWeakCounter;
	
	void ReleaseCurrentReference() {
		if (m_ptr != nullptr)(*m_pWeakCounter).weakCounter --;
		if (m_pWeakCounter != nullptr && (*m_pWeakCounter).weakCounter == 0 && (*m_pWeakCounter).refCounter == 0) {
			delete m_pWeakCounter;
			m_pWeakCounter = nullptr;
			if (m_ptr != nullptr) {
				delete m_ptr;
				m_ptr = nullptr;
			}
		}
	}
	void AquireNewReference(const WeakPtr & i_other) {
		m_ptr = i_other.m_ptr;
		m_pWeakCounter = i_other.m_pWeakCounter;
		if (i_other.m_pWeakCounter != nullptr)(*i_other.m_pWeakCounter).weakCounter++;
	}
};