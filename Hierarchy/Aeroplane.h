#pragma once

#include "Application.h"
#include "HierarchialComponent.h"
#include "HierarchialParent.h"
#include "MeshManager.h"

//*********************************************************************************************
// File:			Aeroplane.h
// Description:		New Aeroplane class extending from HierchialParent and using 
//					HierarchialComponents. Contains logic controlling player input for the plane
//					and handling the camera view when toggled on.
// Notes:			
// Todo:			Add comments for readability
//*********************************************************************************************

__declspec(align(16)) class Aeroplane : public HierarchialParent
{
public:
	Aeroplane(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f, float fRotY = 0.0f);
	~Aeroplane() = default;

	void LoadResources();
	void ReleaseResources();

	void Update(bool bPlayerControl);
	void Draw(XMFLOAT3 camPos);

	//bool LoadShader();
	//void DeleteShader();

private:
	void UpdateMatrices();
	void UpdateCameraMatrix();

	void UpdatePlaneMovement();
	void ResetMovementToZero();

	//void UpdateShader(XMFLOAT3 camPos);

private:
	static bool s_bResourcesReady;

	XMVECTOR m_vForwardVector;
	XMVECTOR m_vCamWorldPos;

	XMMATRIX m_mCamWorldMatrix;

	XMFLOAT4 m_v4CamRot; // Local rotation angles
	XMFLOAT4 m_v4CamOff; // Local offset

	float m_fSpeed;

	bool m_bGunCam;

	//Application::Shader m_shader;

	//ID3D11Buffer* m_pMyAppCBuffer; // our custom buffer resource.
	//int m_psMyAppCBufferSlot; // custom buffer resource binding in PS, discovered by reflection.
	//int m_vsMyAppCBufferSlot; // custom buffer resource binding in VS, discovered by reflection.
	//int m_cameraPosOffset;
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

	float GetMovementSpeed() { return m_fSpeed; };

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

