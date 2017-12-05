#include "Aeroplane.h"


//*********************************************************************************************
//************                           Constructor/Destructor                ****************
//*********************************************************************************************

Aeroplane::Aeroplane()
{
	//Call constructor for parent calss
	HierarchialParent::HierarchialParent();
	
	//Initialise member variables
	m_fSpeed = 0.0f;
	m_bGunCam = false;
	m_bStop = false;

	//Set vector membors to zero
	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();
}

Aeroplane::Aeroplane(float fX, float fY, float fZ, float fRotY)
{
	//Call constructor for parent calss
	HierarchialParent::HierarchialParent();

	//Initialise member variables
	m_fSpeed = 0.0f;
	m_bGunCam = false;
	m_bStop = false;
	
	//Set vector membors to zero
	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	InitialiseComponents();
	InitialiseColours();

	//Set initial positions and rotations for each component
	m_pPlane->SetLocalPosition(fX, fY, fZ);
	m_pPlane->SetLocalRotation(0.0f, fRotY, 0.0f);

	m_pProp->SetLocalPosition(0.0f, 0.0f, 1.9f);

	m_pTurret->SetLocalPosition(0.0f, 1.05f, -1.3f);

	m_pGun->SetLocalPosition(0.0f, 0.5f, 0.0f);

	//Initialise camera offsets and rotation
	m_v4CamOff = XMFLOAT4(0.0f, 4.5f, -15.0f, 0.0f);
	m_v4CamRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

Aeroplane::~Aeroplane()
{
}

//*********************************************************************************************
//************                           Initialisation                        ****************
//*********************************************************************************************


void Aeroplane::InitialiseComponents()
{
	//Add components to hierarchy by loading the mesh, settings it's parent and giving the component a name
	m_pPlane = AddHierarchyComponent(new HierarchialComponent("", MeshManager::GetInstance().LoadResources("Resources/Plane/plane.x", "plane")), "plane");
	m_pProp = AddHierarchyComponent(new HierarchialComponent("plane", MeshManager::GetInstance().LoadResources("Resources/Plane/prop.x", "prop")), "prop");
	m_pTurret = AddHierarchyComponent(new HierarchialComponent("plane", MeshManager::GetInstance().LoadResources("Resources/Plane/turret.x", "turret")), "turret");
	m_pGun = AddHierarchyComponent(new HierarchialComponent("turret", MeshManager::GetInstance().LoadResources("Resources/Plane/gun.x", "gun")), "gun");
}

void Aeroplane::InitialiseColours()
{
	//TODO Allow this to be done though a file instead
	//Set the colours for each component
	m_pPlane->SetColor(0.050f, 0.231f, 0.101f, 1.0f);
	m_pProp->SetColor(0.862f, 0.858f, 0.015f, 1.0f);
	m_pTurret->SetColor(0.345, 0.207f, 0.098f, 1.0f);
	m_pGun->SetColor(0.819f, 0.8190f, 0.717f, 1.0f);
}

//*********************************************************************************************
//************                           Update Functions                      ****************
//*********************************************************************************************

void Aeroplane::Update(bool bPlayerControl)
{
	//If the plane is under player control
	if (bPlayerControl)
	{
		//Get input and update plane movement
		HandlePlaneInput();

		//Reset rotations to zero
		HandlePlaneReset();
	}

	// Apply a forward thrust and limit to a maximum speed of 1
	m_fSpeed += 0.001f;
	if (m_fSpeed > 1)
		m_fSpeed = 1;

	// Rotate propeller and turret
	m_pProp->SetRotationZ(m_pProp->GetLocalRotation().z + 100 * m_fSpeed);

	//If the gun camera isn't enabled
	if (!m_bGunCam)
	{
		//Reset the gun and turrets rotation to zero
		m_pGun->SetLocalRotation(0, 0, 0);
		m_pTurret->SetLocalRotation(0, 0, 0);
	}
	//If we're in gun camera mode
	else
	{
		// Tilt gun up and down as turret rotates
		m_pGun->SetRotationX(sin((float)XMConvertToRadians(m_pTurret->GetLocalRotation().y * 4.0f)) * 10.0f - 10.0f);
		m_pTurret->SetRotationY(m_pTurret->GetLocalRotation().y + 0.1f);
	}

	XMFLOAT4 planePosition;
	//Get current plane position
	XMVECTOR vCurrPos = XMLoadFloat4(&m_pPlane->GetLocalPosition());
	//If we're not in stop mode
	if (!m_bStop)
		//Add the forward vector * speed onto our current position
		vCurrPos += m_vForwardVector * m_fSpeed;
	//Store this position back in a XMFLOAT4
	XMStoreFloat4(&planePosition, vCurrPos);

	//Set the position of the plane
	m_pPlane->SetLocalPosition(planePosition);

	//Update each component's matrices
	UpdateMatrices();
}

void Aeroplane::UpdateMatrices()
{
	//Update each component in the hierarchy
	UpdateHierarchy(false);
	//Update our camera matrix
	UpdateCameraMatrix();

	//Calculate the forward vector
	m_vForwardVector = XMVector4Transform(XMVectorSet(0, 0, 1, 0), m_pPlane->GetWorldMatrix());
}

void Aeroplane::UpdateCameraMatrix()
{
	XMMATRIX mPlaneCameraRot;
	mPlaneCameraRot = XMMatrixRotationY(XMConvertToRadians(m_pPlane->GetLocalRotation().y));

	XMMATRIX mRotX, mRotY, mRotZ, mTrans;

	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4CamRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4CamRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4CamRot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4CamOff));

	//Calculate camWorld Matrix
	m_mCamWorldMatrix = mRotX * mRotY * mRotZ * mTrans;

	m_mCamWorldMatrix *= (!m_bGunCam) ? mPlaneCameraRot * XMMatrixTranslation(m_pPlane->GetLocalPosition().x, m_pPlane->GetLocalPosition().y, m_pPlane->GetLocalPosition().z) : m_pGun->GetWorldMatrix();

	XMVECTOR scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_mCamWorldMatrix);
	m_vCamWorldPos = position;
}


//*********************************************************************************************
//************                           Rendering                             ****************
//*********************************************************************************************

void Aeroplane::Draw(XMFLOAT3 camPos, float mFrameCount)
{
	//Update the shader by passing it the camera position (for specular shading) and 
	//frame count (for modifying lightness)
	UpdateShader(camPos, mFrameCount);

	//Draw the hierarchy for this object
	DrawHierarchy();
}

//*********************************************************************************************
//************                           Player Input / Movement               ****************
//*********************************************************************************************

void Aeroplane::HandlePlaneInput()
{
	//Get the current x rotation of the plane
	float rotX = m_pPlane->GetLocalRotation().x;

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
	
	//Set the new x rotation
	m_pPlane->SetRotationX(rotX);

	//Get the plane's current y and z rotation
	float rotY = m_pPlane->GetLocalRotation().y;
	float rotZ = m_pPlane->GetLocalRotation().z;
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

	m_pPlane->SetRotationY(rotY);
	m_pPlane->SetRotationZ(rotZ);
}

void Aeroplane::HandlePlaneReset()
{

	float rotX = m_pPlane->GetLocalRotation().x;
	float rotZ = m_pPlane->GetLocalRotation().z;

	//If neither left or right is being pressed
	if (!Application::s_pApp->IsKeyPressed('P') && !Application::s_pApp->IsKeyPressed('O'))
	{
		if (rotZ > 0)
		{
			//Update the z rotation so it becomes closer to zero
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

	//If neither up or down is being pressed
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

	m_pPlane->SetRotationX(rotX);
	m_pPlane->SetRotationZ(rotZ);
}

//*********************************************************************************************
//************                           END                                   ****************
//*********************************************************************************************
