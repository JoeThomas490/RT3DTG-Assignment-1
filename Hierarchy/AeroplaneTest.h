#pragma once

#include "Application.h"
#include "HierarchialComponent.h"
#include "HierarchialParent.h"
//*********************************************************************************************
// File:			AeroplaneTest.h
// Description:			
// Notes:
//*********************************************************************************************

__declspec(align(16)) class AeroplaneTest : public HierarchialParent
{
public:
	AeroplaneTest(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f, float fRotY = 0.0f);
	~AeroplaneTest() = default;

	static void LoadResources();
	static void ReleaseResources();

	void Update(bool bPlayerControl);
	void Draw();

private:
	void UpdateMatrices();
	void UpdateCameraMatrix();

	void UpdatePlaneMovement();
	void ResetMovementToZero();

private:
	static CommonMesh* s_pPlaneMesh;
	static CommonMesh* s_pPropMesh;
	static CommonMesh* s_pTurretMesh;
	static CommonMesh* s_pGunMesh;

	static bool s_bResourcesReady;

	HierarchialComponent m_hHullComponent;
	HierarchialComponent m_hPropComponent;
	HierarchialComponent m_hTurretComponent;
	HierarchialComponent m_hGunComponent;

	XMVECTOR m_vForwardVector;
	XMVECTOR m_vCamWorldPos;

	XMMATRIX m_mCamWorldMatrix;

	XMFLOAT4 m_v4CamRot; // Local rotation angles
	XMFLOAT4 m_v4CamOff; // Local offset

	float m_fSpeed;

	bool m_bGunCam;

public :
	XMFLOAT4 GetFocusPosition(void) { return m_hHullComponent.GetLocalPosition(); }
	XMFLOAT4 GetCameraPosition(void)
	{
		XMFLOAT4 v4Pos;
		XMStoreFloat4(&v4Pos, m_vCamWorldPos);
		return v4Pos;
	}

	XMMATRIX GetGunWorldMatrix() { return m_hGunComponent.GetWorldMatrix(); };

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

