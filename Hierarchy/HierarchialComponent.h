#pragma once


#include "Application.h"


//*********************************************************************************************
// File:			HierarchialComponent.h
// Description:		Class to hold data for each component in an animation hiararchy
// Notes:
//*********************************************************************************************

__declspec(align(16)) class HierarchialComponent
{
public:

	HierarchialComponent();
	HierarchialComponent(char* parentNode);
	HierarchialComponent(char* parentNode, CommonMesh* mesh);

	~HierarchialComponent() = default;

	void SetWorldMatrix(XMMATRIX* mWorldMatrix);


	void SetLocalRotation(float fX, float fY, float fZ);
	void SetLocalPosition(float fX, float fY, float fZ);

	void Draw();

	XMMATRIX UpdateLocalMatrix();

private:

	XMMATRIX m_mLocalMatrix;
	XMMATRIX m_mWorldMatrix;

	XMFLOAT4 m_v4Rot;
	XMFLOAT4 m_v4Pos;
	XMFLOAT4 m_vForwardVector;

	CommonMesh* m_mMesh;

	bool m_bIsDrawable;

	char* m_cParentNode;

private:
	XMVECTOR CalculateQuaternion();

public:

	XMMATRIX GetLocalMatrix() { return m_mLocalMatrix; };
	XMMATRIX GetWorldMatrix() { return m_mWorldMatrix; };

	XMFLOAT4 GetLocalPosition() { return m_v4Pos; };
	XMFLOAT4 GetLocalRotation() { return m_v4Rot; };

	char* GetParentNode() { return m_cParentNode; };

	void SetLocalPosition(XMFLOAT4 mPos) { m_v4Pos = mPos; };

	void SetPositionX(float f) { m_v4Pos.x = f; };
	void SetPositionY(float f) { m_v4Pos.y = f; };
	void SetPositionZ(float f) { m_v4Pos.z = f; };

	void SetRotationX(float f) { m_v4Rot.x = f; };
	void SetRotationY(float f) { m_v4Rot.y = f; };
	void SetRotationZ(float f) { m_v4Rot.z = f; };

	void SetMesh(CommonMesh* mesh) { m_mMesh = mesh; m_bIsDrawable = true; };

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

