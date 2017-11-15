#pragma once

#include "Application.h"

/*
BULLET.H
Class holding all the data relavent to the bullet. Handles the spawning, visibility
, lifetime and movement of each bullet


TODO
- Change to a dynamically allocated array (See Application.h and Application.cpp)
- Modify the rotation of the bullet based on the rotation of the gun barrel (Modified in Application.cpp)
*/

__declspec(align(16)) class Bullet
{
public:
	Bullet();
	~Bullet() = default;

	void ResetBullet(const XMMATRIX& mGunWorldMatrix, XMFLOAT4 mPlaneForwardVector, float fSpeed);

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

