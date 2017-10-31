//*********************************************************************************************
// File:			Aeroplane.cpp
// Description:		A very simple class to represent an aeroplane as one object with all the
//					hierarchical components stored internally within the class.
// Module:			Real-Time 3D Techniques for Games
// Created:			Jake - 2010-2011
// Notes:
//*********************************************************************************************

#include "Aeroplane.h"

// Initialise static class variables.
CommonMesh* Aeroplane::s_pPlaneMesh = NULL;
CommonMesh* Aeroplane::s_pPropMesh = NULL;
CommonMesh* Aeroplane::s_pTurretMesh = NULL;
CommonMesh* Aeroplane::s_pGunMesh = NULL;

bool Aeroplane::s_bResourcesReady = false;

Aeroplane::Aeroplane(float fX, float fY, float fZ, float fRotY)
{
	m_mWorldMatrix = XMMatrixIdentity();
	m_mPropWorldMatrix = XMMatrixIdentity();
	m_mTurretWorldMatrix = XMMatrixIdentity();
	m_mGunWorldMatrix = XMMatrixIdentity();
	m_mCamWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0.0f, fRotY, 0.0f, 0.0f);
	m_v4Pos = XMFLOAT4(fX, fY, fZ, 0.0f);

	m_v4PropOff = XMFLOAT4(0.0f, 0.0f, 1.9f, 0.0f);
	m_v4PropRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_v4TurretOff = XMFLOAT4(0.0f, 1.05f, -1.3f, 0.0f);
	m_v4TurretRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_v4GunOff = XMFLOAT4(0.0f, 0.5f, 0.0f, 0.0f);
	m_v4GunRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_v4CamOff = XMFLOAT4(0.0f, 4.5f, -15.0f, 0.0f);
	m_v4CamRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	m_fSpeed = 0.0f;

	m_bGunCam = false;
}

Aeroplane::~Aeroplane(void)
{
}

void Aeroplane::SetWorldPosition(float fX, float fY, float fZ)
{
	m_v4Pos = XMFLOAT4(fX, fY, fZ, 0.0f);
	UpdateMatrices();
}

XMMATRIX Aeroplane::GetTurretWorldMatrix()
{
	return m_mTurretWorldMatrix;
}

void Aeroplane::UpdateMatrices(void)
{
	XMMATRIX mRotX, mRotY, mRotZ, mTrans;
	XMMATRIX mPlaneCameraRot, mForwardMatrix;

	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4Rot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4Rot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4Rot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4Pos));

	XMMATRIX planeParentMat = mRotZ * mRotX * mRotY * mTrans;
	//------


	//Calculate propeller local matrix
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4PropRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4PropRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4PropRot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4PropOff));

	XMMATRIX propMatrix = mRotX * mRotY * mRotZ * mTrans;
	//-----

	//Calculate turret local matrix
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4TurretRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4TurretRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4TurretRot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4TurretOff));

	XMMATRIX turretMatrix = mRotX * mRotY * mRotZ * mTrans;
	//-----

	//Calculate gun local matrix
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4GunRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4GunRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4GunRot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4GunOff));
	XMMATRIX gunMatrix = mRotX * mRotY * mRotZ * mTrans;
	//-----

	m_mWorldMatrix = planeParentMat;
	m_mPropWorldMatrix = propMatrix * planeParentMat;
	m_mTurretWorldMatrix = turretMatrix * planeParentMat;
	m_mGunWorldMatrix = gunMatrix * m_mTurretWorldMatrix;

	//Also calculate mPlaneCameraRot which ignores rotations in Z and X for the camera to parent to
	mPlaneCameraRot = XMMatrixRotationY(XMConvertToRadians(m_v4Rot.y));
	//Calculate camera rotation like you would the others
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4CamRot.x));
	mRotY = XMMatrixRotationX(XMConvertToRadians(m_v4CamRot.y));
	mRotZ = XMMatrixRotationX(XMConvertToRadians(m_v4CamRot.z));
	//Calculate camera translation
	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4CamOff));

	//Calculate camWorld Matrix
	m_mCamWorldMatrix = mRotX * mRotY * mRotZ * mTrans;
	m_mCamWorldMatrix *= (!m_bGunCam) ? mPlaneCameraRot * XMMatrixTranslation(m_v4Pos.x, m_v4Pos.y, m_v4Pos.z) : m_mGunWorldMatrix;

	XMVECTOR scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_mCamWorldMatrix);
	m_vCamWorldPos = position;

	m_vForwardVector = XMVector4Transform(XMVectorSet(0, 0, 1, 0), m_mWorldMatrix);

}

void Aeroplane::Update(bool bPlayerControl)
{
	if (bPlayerControl)
	{
		UpdatePlaneMovement();
		ResetMovementToZero();
		

	} // End of if player control

	// Apply a forward thrust and limit to a maximum speed of 1
	m_fSpeed += 0.001f;

	if (m_fSpeed > 1)
		m_fSpeed = 1;

	// Rotate propeller and turret
	m_v4PropRot.z += 100 * m_fSpeed;
	m_v4TurretRot.y += 0.1f;

	// Tilt gun up and down as turret rotates
	m_v4GunRot.x = (sin((float)XMConvertToRadians(m_v4TurretRot.y * 4.0f)) * 10.0f) - 10.0f;

	UpdateMatrices();

	// Move Forward
	XMVECTOR vCurrPos = XMLoadFloat4(&m_v4Pos);
	vCurrPos += m_vForwardVector * m_fSpeed;
	XMStoreFloat4(&m_v4Pos, vCurrPos);
}

void Aeroplane::UpdatePlaneMovement()
{
	// Step 1: Make the plane pitch upwards when you press "Q" and return to level when released
	// Maximum pitch = 60 degrees
	if (Application::s_pApp->IsKeyPressed('Q'))
	{
		//if (m_fSpeed > 0.5f)
		{
			m_v4Rot.x -= 3.0f;
			if (m_v4Rot.x < -60.0f)
			{
				m_v4Rot.x = -60.0f;
			}
		}
	}

	// Step 2: Make the plane pitch downwards when you press "A" and return to level when released
	// You can also impose a take off speed of 0.5 if you like
	// Minimum pitch = -60 degrees
	if (Application::s_pApp->IsKeyPressed('A'))
	{
		//if (m_fSpeed > 0.5f)
		{
			m_v4Rot.x += 3.0f;
			if (m_v4Rot.x > 60.0f)
			{
				m_v4Rot.x = 60.0f;
			}
		}
	}

	// Step 3: Make the plane yaw and roll left when you press "O" and return to level when released
	// Maximum roll = 20 degrees
	if (Application::s_pApp->IsKeyPressed('O'))
	{
		m_v4Rot.z += 1.5f;
		m_v4Rot.y -= 1.5f;
		if (m_v4Rot.z > 20)
		{
			m_v4Rot.z = 20.0f;
		}
	}
	// Step 4: Make the plane yaw and roll right when you press "P" and return to level when released
	// Minimum roll = -20 degrees

	if (Application::s_pApp->IsKeyPressed('P'))
	{
		m_v4Rot.z -= 1.5f;
		m_v4Rot.y += 1.5f;
		if (m_v4Rot.z < -20)
		{
			m_v4Rot.z = -20.0f;
		}
	}
}

void Aeroplane::ResetMovementToZero()
{
	if (!Application::s_pApp->IsKeyPressed('P') && !Application::s_pApp->IsKeyPressed('O'))
	{
		if (m_v4Rot.z > 0)
		{
			m_v4Rot.z -= 4.0f;
			if (m_v4Rot.z < 0)
			{
				m_v4Rot.z = 0;
			}
		}
		if (m_v4Rot.z < 0)
		{
			m_v4Rot.z += 4.0f;
			if (m_v4Rot.z > 0)
			{
				m_v4Rot.z = 0;
			}
		}
	}

	if (!Application::s_pApp->IsKeyPressed('A') && !Application::s_pApp->IsKeyPressed('Q'))
	{
		if (m_v4Rot.x > 0)
		{
			m_v4Rot.x -= 2.5f;
			if (m_v4Rot.x < 0)
			{
				m_v4Rot.x = 0;
			}
		}
		if (m_v4Rot.x < 0)
		{
			m_v4Rot.x += 2.5f;
			if (m_v4Rot.x > 0)
			{
				m_v4Rot.x = 0;
			}
		}
	}
}

void Aeroplane::LoadResources(void)
{
	s_pPlaneMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/plane.x");
	s_pPropMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/prop.x");
	s_pTurretMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/turret.x");
	s_pGunMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/gun.x");
}

void Aeroplane::ReleaseResources(void)
{
	delete s_pPlaneMesh;
	delete s_pPropMesh;
	delete s_pTurretMesh;
	delete s_pGunMesh;
}

void Aeroplane::Draw(void)
{
	Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
	s_pPlaneMesh->Draw();

	Application::s_pApp->SetWorldMatrix(m_mPropWorldMatrix);
	s_pPropMesh->Draw();

	Application::s_pApp->SetWorldMatrix(m_mTurretWorldMatrix);
	s_pTurretMesh->Draw();

	Application::s_pApp->SetWorldMatrix(m_mGunWorldMatrix);
	s_pGunMesh->Draw();
}
