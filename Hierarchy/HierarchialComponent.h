#pragma once


#include "Application.h"


__declspec(align(16)) class HierarchialComponent
{
public:
	HierarchialComponent();
	virtual ~HierarchialComponent() = default;


private:
	virtual void UpdateMatrices() = 0;

	XMMATRIX m_mLocalMatrix;
	XMMATRIX m_mWorldMatrix;

public:
	XMMATRIX GetLocalMatrix() { m_mLocalMatrix; };
	XMMATRIX GetWorldMatrix() { m_mWorldMatrix; };


	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

