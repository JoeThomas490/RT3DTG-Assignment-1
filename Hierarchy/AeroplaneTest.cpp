#include "AeroplaneTest.h"

bool AeroplaneTest::s_bResourcesReady = false;

AeroplaneTest::AeroplaneTest(float fX, float fY, float fZ, float fRotY)
{
	m_fSpeed = 0.0f;
	m_bGunCam = false;

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();


	AddHierarchyComponent(new HierarchialComponent("", MeshManager::GetInstance().LoadResources("Resources/Plane/plane.x", "plane")), "plane");
	AddHierarchyComponent(new HierarchialComponent("plane", MeshManager::GetInstance().LoadResources("Resources/Plane/prop.x", "prop")), "prop");
	AddHierarchyComponent(new HierarchialComponent("plane", MeshManager::GetInstance().LoadResources("Resources/Plane/turret.x", "turret")), "turret");
	AddHierarchyComponent(new HierarchialComponent("turret", MeshManager::GetInstance().LoadResources("Resources/Plane/gun.x", "gun")), "gun");

	GetHiararchyComponentFromTag("plane")->SetLocalPosition(fX, fY, fZ);
	GetHiararchyComponentFromTag("plane")->SetLocalRotation(0.0f, fRotY, 0.0f);

	GetHiararchyComponentFromTag("prop")->SetLocalPosition(0.0f, 0.0f, 1.9f);

	GetHiararchyComponentFromTag("turret")->SetLocalPosition(0.0f, 1.05f, -1.3f);

	GetHiararchyComponentFromTag("gun")->SetLocalPosition(0.0f, 0.5f, 0.0f);

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
	HierarchialComponent* mProp = GetHiararchyComponentFromTag("prop");
	mProp->SetRotationZ(mProp->GetLocalRotation().z + 100 * m_fSpeed);

	HierarchialComponent* mTurret = GetHiararchyComponentFromTag("turret");
	mTurret->SetRotationY(mTurret->GetLocalRotation().y + 0.1f);

	// Tilt gun up and down as turret rotates
	HierarchialComponent* mGun = GetHiararchyComponentFromTag("gun");
	mGun->SetRotationX(sin((float)XMConvertToRadians(mTurret->GetLocalRotation().y * 4.0f)) * 10.0f - 10.0f);

	UpdateMatrices();

	// Move Forward
	XMFLOAT4 planePosition;
	XMVECTOR vCurrPos = XMLoadFloat4(&GetHiararchyComponentFromTag("plane")->GetLocalPosition());
	vCurrPos += m_vForwardVector * m_fSpeed;
	XMStoreFloat4(&planePosition, vCurrPos);

	GetHiararchyComponentFromTag("plane")->SetLocalPosition(planePosition);
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

	m_vForwardVector = XMVector4Transform(XMVectorSet(0, 0, 1, 0), GetHiararchyComponentFromTag("plane")->GetWorldMatrix());
}

//TODO Cleanup by creating a camera component and adding to hierarchy
void AeroplaneTest::UpdateCameraMatrix()
{
	XMMATRIX mPlaneCameraRot;
	mPlaneCameraRot = XMMatrixRotationY(XMConvertToRadians(GetHiararchyComponentFromTag("plane")->GetLocalRotation().y));

	XMMATRIX mRotX, mRotY, mRotZ, mTrans;

	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4CamRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4CamRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4CamRot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4CamOff));

	//Calculate camWorld Matrix
	m_mCamWorldMatrix = mRotX * mRotY * mRotZ * mTrans;

	m_mCamWorldMatrix *= (!m_bGunCam) ? mPlaneCameraRot * XMMatrixTranslation(GetHiararchyComponentFromTag("plane")->GetLocalPosition().x, GetHiararchyComponentFromTag("plane")->GetLocalPosition().y, GetHiararchyComponentFromTag("plane")->GetLocalPosition().z) : GetHiararchyComponentFromTag("gun")->GetWorldMatrix();

	XMVECTOR scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_mCamWorldMatrix);
	m_vCamWorldPos = position;
}

void AeroplaneTest::UpdatePlaneMovement()
{
	float rotX = GetHiararchyComponentFromTag("plane")->GetLocalRotation().x;
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
	GetHiararchyComponentFromTag("plane")->SetRotationX(rotX);

	float rotY = GetHiararchyComponentFromTag("plane")->GetLocalRotation().y;
	float rotZ = GetHiararchyComponentFromTag("plane")->GetLocalRotation().z;
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

	GetHiararchyComponentFromTag("plane")->SetRotationY(rotY);
	GetHiararchyComponentFromTag("plane")->SetRotationZ(rotZ);
}

void AeroplaneTest::ResetMovementToZero()
{

	float rotX = GetHiararchyComponentFromTag("plane")->GetLocalRotation().x;
	float rotZ = GetHiararchyComponentFromTag("plane")->GetLocalRotation().z;
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

	GetHiararchyComponentFromTag("plane")->SetRotationX(rotX);
	GetHiararchyComponentFromTag("plane")->SetRotationZ(rotZ);
}
