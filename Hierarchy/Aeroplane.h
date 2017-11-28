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
// Todo:			
//*********************************************************************************************

__declspec(align(16)) class Aeroplane : public HierarchialParent
{
public:

	Aeroplane(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f, float fRotY = 0.0f);
	virtual ~Aeroplane() = default;

	void Update(bool bPlayerControl);
	void Draw(XMFLOAT3 camPos, float mFrameCount);



	//Whether the plane should move or not (debug feature)
	bool m_bStop;

private:
	void UpdateMatrices();
	void UpdateCameraMatrix();

	void UpdatePlaneMovement();
	void ResetMovementToZero();

private:
	static bool s_bResourcesReady;

	XMVECTOR m_vForwardVector;
	XMVECTOR m_vCamWorldPos; //Position for camera

	XMMATRIX m_mCamWorldMatrix; //World matrix for camera

	XMFLOAT4 m_v4CamRot; // Local rotation angles
	XMFLOAT4 m_v4CamOff; // Local offset

	float m_fSpeed; //Movement speed

	bool m_bGunCam; //Whether the gun camera is active or not

	//Pointers to each component of the plane
	HierarchialComponent* m_pPlane;
	HierarchialComponent* m_pProp;
	HierarchialComponent* m_pGun;
	HierarchialComponent* m_pTurret;
public:
	//Get focus position for camera i.e the position of the plane
	XMFLOAT4 GetFocusPosition(void) { return GetHiararchyComponentFromTag("plane")->GetLocalPosition(); }
	//Get current camera position
	XMFLOAT4 GetCameraPosition(void)
	{
		XMFLOAT4 v4Pos;
		XMStoreFloat4(&v4Pos, m_vCamWorldPos);
		return v4Pos;
	}

	//Get world matrix for the gun component (used for bullet positioning and rotation)
	XMMATRIX GetGunWorldMatrix() { return GetHiararchyComponentFromTag("gun")->GetWorldMatrix(); };
	
	//Get the current forward vector of the plane
	XMFLOAT4 GetForwardVector()
	{
		XMFLOAT4 v4Forward;
		XMStoreFloat4(&v4Forward, XMVector3Normalize(m_vForwardVector));
		return v4Forward;
	}

	//Set the gun camera bool
	void SetGunCamera(bool value) { m_bGunCam = value; }


	//---------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------
	//Operator overloads for new and delete so they make sure the object is alligned to a 16 byte boundary
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

