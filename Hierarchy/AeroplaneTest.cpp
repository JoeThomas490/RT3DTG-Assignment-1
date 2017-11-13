#include "AeroplaneTest.h"

bool AeroplaneTest::s_bResourcesReady = false;

AeroplaneTest::AeroplaneTest(float fX, float fY, float fZ, float fRotY)
{
	m_fSpeed = 0.0f;
	m_bGunCam = false;

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	AddComponent(&m_hHullComponent, -1);
	AddComponent(&m_hPropComponent, 0);
	AddComponent(&m_hTurretComponent, 0);
	AddComponent(&m_hGunComponent, 2);


	//AddHierarchyComponent(new HierarchialComponent(-1, MeshManager::GetInstance().LoadResources("Resources/Plane/plane.x", "plane")), "plane");

	m_hHullComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Plane/plane.x","plane"));

	m_hPropComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Plane/prop.x", "prop"));
	m_hTurretComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Plane/turret.x", "turret"));
	m_hGunComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Plane/gun.x", "gun"));

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
}

void AeroplaneTest::ReleaseResources()
{
}

void AeroplaneTest::Update(bool bPlayerControl)
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

	m_hPropComponent.SetRotationZ(m_hPropComponent.GetLocalRotation().z + 100 * m_fSpeed);
	m_hTurretComponent.SetRotationY(m_hTurretComponent.GetLocalRotation().y + 0.1f);

	// Tilt gun up and down as turret rotates
	m_hGunComponent.SetRotationX(sin((float)XMConvertToRadians(m_hTurretComponent.GetLocalRotation().y * 4.0f)) * 10.0f - 10.0f);

	UpdateMatrices();

	// Move Forward
	XMFLOAT4 planePosition;
	XMVECTOR vCurrPos = XMLoadFloat4(&m_hHullComponent.GetLocalPosition());
	vCurrPos += m_vForwardVector * m_fSpeed;
	XMStoreFloat4(&planePosition, vCurrPos);

	m_hHullComponent.SetLocalPosition(planePosition);
}

void AeroplaneTest::Draw()
{
	DrawHierarchy();
}

void AeroplaneTest::UpdateMatrices()
{
	XMMATRIX mPlaneCameraRot, mForwardMatrix;

	UpdateHierarchy();
	UpdateCameraMatrix();

	m_vForwardVector = XMVector4Transform(XMVectorSet(0, 0, 1, 0), m_hHullComponent.GetWorldMatrix());
}

//TODO Cleanup by creating a camera component and adding to hierarchy
void AeroplaneTest::UpdateCameraMatrix()
{
	XMMATRIX mPlaneCameraRot;
	mPlaneCameraRot = XMMatrixRotationY(XMConvertToRadians(m_hHullComponent.GetLocalRotation().y));

	XMMATRIX mRotX, mRotY, mRotZ, mTrans;

	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4CamRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4CamRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4CamRot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4CamOff));

	//Calculate camWorld Matrix
	m_mCamWorldMatrix = mRotX * mRotY * mRotZ * mTrans;

	m_mCamWorldMatrix *= (!m_bGunCam) ? mPlaneCameraRot * XMMatrixTranslation(m_hHullComponent.GetLocalPosition().x, m_hHullComponent.GetLocalPosition().y, m_hHullComponent.GetLocalPosition().z) : m_hGunComponent.GetWorldMatrix();

	XMVECTOR scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_mCamWorldMatrix);
	m_vCamWorldPos = position;
}

void AeroplaneTest::UpdatePlaneMovement()
{
	float rotX = m_hHullComponent.GetLocalRotation().x;
	// Step 1: Make the plane pitch upwards when you press "Q" and return to level when released
	// Maximum pitch = 60 degrees
	if (Application::s_pApp->IsKeyPressed('Q'))
	{
		//if (m_fSpeed > 0.5f)
		{
			rotX -= 3.0f;
			if (rotX < -60.0f)
			{
				rotX = -60.0f;
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
			rotX += 3.0f;
			if (rotX > 60.0f)
			{
				rotX = 60.0f;
			}
		}
	}
	m_hHullComponent.SetRotationX(rotX);

	float rotY = m_hHullComponent.GetLocalRotation().y;
	float rotZ = m_hHullComponent.GetLocalRotation().z;
	// Step 3: Make the plane yaw and roll left when you press "O" and return to level when released
	// Maximum roll = 20 degrees
	if (Application::s_pApp->IsKeyPressed('O'))
	{

		rotZ += 1.5f;
		rotY -= 1.5f;

		if (rotZ > 20)
		{
			rotZ = 20.0f;
		}
	}
	// Step 4: Make the plane yaw and roll right when you press "P" and return to level when released
	// Minimum roll = -20 degrees

	if (Application::s_pApp->IsKeyPressed('P'))
	{
		rotZ -= 1.5f;
		rotY += 1.5f;

		if (rotZ < -20)
		{
			rotZ = -20.0f;
		}
	}

	m_hHullComponent.SetRotationY(rotY);
	m_hHullComponent.SetRotationZ(rotZ);
}

void AeroplaneTest::ResetMovementToZero()
{

	float rotX = m_hHullComponent.GetLocalRotation().x;
	float rotZ = m_hHullComponent.GetLocalRotation().z;
	if (!Application::s_pApp->IsKeyPressed('P') && !Application::s_pApp->IsKeyPressed('O'))
	{
		if (rotZ > 0)
		{
			rotZ -= 4.0f;
			if (rotZ < 0)
			{
				rotZ = 0;
			}
		}
		if (rotZ < 0)
		{
			rotZ += 4.0f;
			if (rotZ > 0)
			{
				rotZ = 0;
			}
		}
	}

	if (!Application::s_pApp->IsKeyPressed('A') && !Application::s_pApp->IsKeyPressed('Q'))
	{
		if (rotX > 0)
		{
			rotX -= 2.5f;
			if (rotX < 0)
			{
				rotX = 0;
			}
		}
		if (rotX < 0)
		{
			rotX += 2.5f;
			if (rotX > 0)
			{
				rotX = 0;
			}
		}
	}

	m_hHullComponent.SetRotationX(rotX);
	m_hHullComponent.SetRotationZ(rotZ);
}
