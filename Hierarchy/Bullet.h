#pragma once

#include "Application.h"

//*********************************************************************************************
// File:			Bullet.h
// Description:		Class holding logic for a singular bullet including visibility, lifetime, 
//					spawning and movement. Used inside Application.cpp inside an array of 
//					pooled bullet that are reset whenever the space bar is pressed.
// Notes:			
// Todo:			- Convert to using the mesh manager and hiearchialComponent to clean up
//					
//*********************************************************************************************


__declspec(align(16)) class Bullet
{
public:
	Bullet();
	~Bullet();

	//Function to be called when "creating a new bullet"
	void ResetBullet(const XMMATRIX& mGunWorldMatrix, XMFLOAT4 mPlaneForwardVector);

	//Load bullet mesh
	static void LoadResources();
	//Release bullet mesh
	static void ReleaseResources();

	void Update();
	void Draw();

private:
	void UpdateMatrices();

private:

	//Pointer to the bullet mesh
	static CommonMesh* s_pBulletMesh;

	//Is the bullet visible? If not don't draw or update
	bool m_bIsVisible;

	//Local rotation in euler angles
	XMFLOAT4 m_v4Rot;
	//Local position
	XMFLOAT4 m_v4Pos;
	//Forward vector of bullet
	XMFLOAT4 m_v4ForwardVector;

	//Movement speed of bullet (not used for now)
	float m_fMovementSpeed;
	//Lifetime of bullet in frames
	float m_fLifeTime;

	//Bullet's overall world matrix
	XMMATRIX m_mWorldMatrix;

	//Local offset of bullet when spawning from end of gun
	XMVECTOR m_vBulletOffset;


public:

	bool GetIsVisible() { return m_bIsVisible; };
	void SetIsVisibile(bool b) { m_bIsVisible = b; };

	XMFLOAT4 GetPosition() { return m_v4Pos; };


	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

