#pragma once


#include "Application.h"


__declspec(align(16)) class HierarchialComponent
{
public:
	HierarchialComponent();
	virtual ~HierarchialComponent() = default;

	void SetWorldMatrix(XMMATRIX* mWorldMatrix);

	void SetLocalRotation(float fX, float fY, float fZ);
	void SetLocalPosition(float fX, float fY, float fZ);

	void Draw(CommonMesh* mesh);

	XMMATRIX UpdateLocalMatrix();

private:

	XMMATRIX m_mLocalMatrix;
	XMMATRIX m_mWorldMatrix;

	XMFLOAT4 m_v4Rot;
	XMFLOAT4 m_v4Pos;
	XMFLOAT4 m_vForwardVector;

public:

	XMMATRIX GetLocalMatrix() { return m_mLocalMatrix; };
	XMMATRIX GetWorldMatrix() { return m_mWorldMatrix; };

	XMFLOAT4 GetLocalPosition() { return m_v4Pos; };
	XMFLOAT4 GetLocalRotation() { return m_v4Rot; };

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

