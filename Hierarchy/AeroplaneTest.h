#pragma once

#include "Application.h"
#include "HierarchialComponent.h"
//*********************************************************************************************
// File:			AeroplaneTest.h
// Description:			
// Notes:
//*********************************************************************************************

__declspec(align(16)) class AeroplaneTest
{
public:
	AeroplaneTest(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f, float fRotY = 0.0f);
	~AeroplaneTest() = default;

	static void LoadResources();
	static void ReleaseResources();

	void Update(bool bPlayerControl);
	void Draw();

	void SetWorldPosition(float fX, float fY, float fZ);

private:
	void UpdateMatrices();
	void UpdateCameraMatrix();

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
};

