#include "AeroplaneTest.h"

CommonMesh* AeroplaneTest::s_pPlaneMesh = nullptr;
CommonMesh* AeroplaneTest::s_pPropMesh = nullptr;
CommonMesh* AeroplaneTest::s_pTurretMesh = nullptr;
CommonMesh* AeroplaneTest::s_pGunMesh = nullptr;

bool AeroplaneTest::s_bResourcesReady = false;

AeroplaneTest::AeroplaneTest(float fX, float fY, float fZ, float fRotY)
{
	m_fSpeed = 0.0f;
	m_bGunCam = false;

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	m_hHullComponent.SetLocalPosition(fX, fY, fZ);
	m_hHullComponent.SetLocalRotation(0.0f, fRotY, 0.0f);

	m_hPropComponent.SetLocalPosition(0.0f, 0.0f, 1.9f);

	m_hTurretComponent.SetLocalPosition(0.0f, 1.05f, -1.3f);

	m_hGunComponent.SetLocalPosition(0.0f, 0.5f, 0.0f);

	m_v4CamOff = XMFLOAT4(0.0f, 4.5f, -15.0f, 0.0f);
	m_v4CamRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}


void AeroplaneTest::LoadResources()
{
	s_pPlaneMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/plane.x");
	s_pPropMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/prop.x");
	s_pTurretMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/turret.x");
	s_pGunMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/gun.x");
}

void AeroplaneTest::ReleaseResources()
{
	delete s_pPlaneMesh;
	delete s_pPropMesh;
	delete s_pTurretMesh;
	delete s_pGunMesh;
}

void AeroplaneTest::Update(bool bPlayerControl)
{
	if (bPlayerControl)
	{
		//UpdatePlaneMovement();
		//ResetMovementToZero();


	} // End of if player control

	  // Apply a forward thrust and limit to a maximum speed of 1
	m_fSpeed += 0.001f;

	if (m_fSpeed > 1)
		m_fSpeed = 1;

	// Rotate propeller and turret
	//m_v4PropRot.z += 100 * m_fSpeed;
	//m_v4TurretRot.y += 0.1f;

	// Tilt gun up and down as turret rotates
	//m_v4GunRot.x = (sin((float)XMConvertToRadians(m_v4TurretRot.y * 4.0f)) * 10.0f) - 10.0f;

	UpdateMatrices();

	// Move Forward
	//XMVECTOR vCurrPos = XMLoadFloat4(&m_v4Pos);
	//vCurrPos += m_vForwardVector * m_fSpeed;
	//XMStoreFloat4(&m_v4Pos, vCurrPos);
}

void AeroplaneTest::Draw()
{
	m_hHullComponent.Draw(s_pPlaneMesh);
	m_hPropComponent.Draw(s_pPropMesh);
	m_hTurretComponent.Draw(s_pTurretMesh);
	m_hGunComponent.Draw(s_pGunMesh);
}

void AeroplaneTest::UpdateMatrices()
{
	XMMATRIX mPlaneCameraRot, mForwardMatrix;

	UpdateCameraMatrix();

	m_hHullComponent.UpdateLocalMatrix();
	m_hPropComponent.UpdateLocalMatrix();
	m_hGunComponent.UpdateLocalMatrix();
	m_hTurretComponent.UpdateLocalMatrix();

	XMMATRIX mHullWorldMatrix = m_hHullComponent.GetLocalMatrix();
	m_hHullComponent.SetWorldMatrix(&mHullWorldMatrix);

	XMMATRIX mPropWorldMatrix = m_hPropComponent.GetLocalMatrix() * m_hHullComponent.GetWorldMatrix();
	m_hPropComponent.SetWorldMatrix(&mPropWorldMatrix);

	XMMATRIX mTurretWorldMatrix = m_hTurretComponent.GetLocalMatrix() * m_hHullComponent.GetWorldMatrix();
	m_hTurretComponent.SetWorldMatrix(&mTurretWorldMatrix);

	XMMATRIX mGunWorldMatrix = m_hGunComponent.GetLocalMatrix() * m_hTurretComponent.GetWorldMatrix();
	m_hGunComponent.SetWorldMatrix(&mGunWorldMatrix);

	m_vForwardVector = XMVector4Transform(XMVectorSet(0, 0, 1, 0), m_hHullComponent.GetWorldMatrix());
}

void AeroplaneTest::UpdateCameraMatrix()
{
	XMMATRIX mPlaneCameraRot;
	mPlaneCameraRot = XMMatrixRotationY(XMConvertToRadians(m_hHullComponent.GetLocalRotation().y));

	XMMATRIX mRotX, mRotY, mRotZ, mTrans;

	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4CamRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4CamRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4CamRot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4CamOff));

	////Calculate camWorld Matrix
	m_mCamWorldMatrix = mRotX * mRotY * mRotZ * mTrans;

	m_mCamWorldMatrix *= (!m_bGunCam) ? mPlaneCameraRot * XMMatrixTranslation(m_hHullComponent.GetLocalPosition().x, m_hHullComponent.GetLocalPosition().y, m_hHullComponent.GetLocalPosition().z) : m_hGunComponent.GetWorldMatrix();

	XMVECTOR scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_mCamWorldMatrix);
	m_vCamWorldPos = position;
}
