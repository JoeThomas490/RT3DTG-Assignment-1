#pragma once

#include "Application.h"

//*********************************************************************************************
// File:			Bullet.h
// Description:		Class holding logic for a singular bullet including visibility, lifetime, 
//					spawning and movement. Used inside Application.cpp inside an array of 
//					pooled bullet that are reset whenever the space bar is pressed.
// Notes:			
// Todo:			- Convert to using the mesh manager and hiearchialComponent to clean up
//					- Add comments
//*********************************************************************************************


__declspec(align(16)) class Bullet
{
public:
	Bullet();
	~Bullet() = default;

	void ResetBullet(const XMMATRIX& mGunWorldMatrix, XMFLOAT4 mPlaneForwardVector, float fPlaneSpeed);

	static void LoadResources();
	static void ReleaseResources();

	void Update();
	void Draw();

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	XMFLOAT4 GetPosition() { return m_v4Pos; };


private:
	void UpdateMatrices();

private:
	static CommonMesh* s_pBulletMesh;

	static bool s_bResourcesReady;

	bool m_bIsVisible;
	bool m_bIsReadyToUpdate;

	XMFLOAT4 m_v4Rot;
	XMFLOAT4 m_v4Pos;

	XMFLOAT4 m_v4ForwardVector;

	float m_fMovementSpeed;

	float m_fLifeTime;

	XMMATRIX m_mWorldMatrix;

	XMVECTOR m_vBulletOffset;


public:

	bool GetIsVisible() { return m_bIsVisible; };
	void SetIsVisibile(bool b) { m_bIsVisible = b; };

	bool IsReadyToUpdate() { return m_bIsReadyToUpdate; };

};

