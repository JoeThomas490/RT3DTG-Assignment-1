#pragma once

#include "Application.h"

__declspec(align(16)) class Bullet
{
public:
	Bullet(XMFLOAT4 mPos, XMFLOAT4 mRot, float fSpeed);
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

	XMFLOAT4 m_v4Rot;
	XMFLOAT4 m_v4Pos;

	XMVECTOR m_vForwardVector;

	float m_fMovementSpeed;

	XMMATRIX m_mWorldMatrix;
};

