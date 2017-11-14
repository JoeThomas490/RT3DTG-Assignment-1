#pragma once

#include "Application.h"
#include "HierarchialComponent.h"
#include "HierarchialParent.h"
#include "MeshManager.h"
//*********************************************************************************************
// File:			AeroplaneTest.h
// Description:		New Aeroplane class extending from HierchialParent and using 
//					HierarchialComponents
// Notes:			Need to move into old Aeroplane class
//*********************************************************************************************

__declspec(align(16)) class AeroplaneTest : public HierarchialParent
{
public:
	AeroplaneTest(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f, float fRotY = 0.0f);
	~AeroplaneTest() = default;

	void LoadResources();
	void ReleaseResources();

	void Update(bool bPlayerControl);
	void Draw();

private:
	void UpdateMatrices();
	void UpdateCameraMatrix();

	void UpdatePlaneMovement();
	void ResetMovementToZero();

private:
	static bool s_bResourcesReady;

	XMVECTOR m_vForwardVector;
	XMVECTOR m_vCamWorldPos;

	XMMATRIX m_mCamWorldMatrix;

	XMFLOAT4 m_v4CamRot; // Local rotation angles
	XMFLOAT4 m_v4CamOff; // Local offset

	float m_fSpeed;

	bool m_bGunCam;

public:
	XMFLOAT4 GetFocusPosition(void) { return GetHiararchyComponentFromTag("plane")->GetLocalPosition(); }
	XMFLOAT4 GetCameraPosition(void)
	{
		XMFLOAT4 v4Pos;
		XMStoreFloat4(&v4Pos, m_vCamWorldPos);
		return v4Pos;
	}

	XMMATRIX GetGunWorldMatrix() { return GetHiararchyComponentFromTag("gun")->GetWorldMatrix(); };
	XMFLOAT4 GetGunRotation() { return GetHiararchyComponentFromTag("turret")->GetLocalRotation(); };	

	XMFLOAT4 GetForwardVector()
	{
		XMFLOAT4 v4Forward;
		XMStoreFloat4(&v4Forward, XMVector3Normalize(m_vForwardVector));
		return v4Forward;
	}

	void SetGunCamera(bool value) { m_bGunCam = value; }

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

