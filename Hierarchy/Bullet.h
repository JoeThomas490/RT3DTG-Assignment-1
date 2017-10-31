#pragma once

#include "Application.h"

__declspec(align(16)) class Bullet
{
public:
	Bullet(XMFLOAT4 mPos, XMFLOAT4 mRot, XMVECTOR mGunForwardVector, XMFLOAT4 mPlaneForwardVector, float fSpeed);
	~Bullet();


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

	XMFLOAT4 m_v4Rot;
	XMFLOAT4 m_v4Pos;

	XMFLOAT4 bulletStartPosm_v4BulletOff; // Local offset

	XMFLOAT4 m_v4ForwardVector;

	float m_fMovementSpeed;

	XMMATRIX m_mWorldMatrix;


public:

	bool GetIsVisible() { return m_bIsVisible; };
	void SetIsVisibile(bool b) { m_bIsVisible = b; };
};

