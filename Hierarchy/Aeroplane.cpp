#include "Aeroplane.h"

bool Aeroplane::s_bResourcesReady = false;

Aeroplane::Aeroplane(float fX, float fY, float fZ, float fRotY)
{
	HierarchialParent();
	m_fSpeed = 0.0f;
	m_bGunCam = false;

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	//m_pMyAppCBuffer = NULL;


	AddHierarchyComponent(new HierarchialComponent("", MeshManager::GetInstance().LoadResources("Resources/Plane/plane.x", "plane")), "plane");
	AddHierarchyComponent(new HierarchialComponent("plane", MeshManager::GetInstance().LoadResources("Resources/Plane/prop.x", "prop")), "prop");
	AddHierarchyComponent(new HierarchialComponent("plane", MeshManager::GetInstance().LoadResources("Resources/Plane/turret.x", "turret")), "turret");
	AddHierarchyComponent(new HierarchialComponent("turret", MeshManager::GetInstance().LoadResources("Resources/Plane/gun.x", "gun")), "gun");

	GetHiararchyComponentFromTag("plane")->SetLocalPosition(fX, fY, fZ);
	GetHiararchyComponentFromTag("plane")->SetLocalRotation(0.0f, fRotY, 0.0f);

	GetHiararchyComponentFromTag("prop")->SetLocalPosition(0.0f, 0.0f, 1.9f);

	GetHiararchyComponentFromTag("turret")->SetLocalPosition(0.0f, 1.05f, -1.3f);

	GetHiararchyComponentFromTag("gun")->SetLocalPosition(0.0f, 0.5f, 0.0f);

	GetHiararchyComponentFromTag("plane")->SetColor(0.050f, 0.231f, 0.101f, 1.0f);
	GetHiararchyComponentFromTag("prop")->SetColor(0.862f, 0.858f, 0.015f, 1.0f);
	GetHiararchyComponentFromTag("turret")->SetColor(0.345, 0.207f, 0.098f, 1.0f);
	GetHiararchyComponentFromTag("gun")->SetColor(0.819f, 0.8190f, 0.717f, 1.0f);

	m_v4CamOff = XMFLOAT4(0.0f, 4.5f, -15.0f, 0.0f);
	m_v4CamRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);


	LoadShader();
}


void Aeroplane::LoadResources()
{
}

void Aeroplane::ReleaseResources()
{
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
	HierarchialComponent* mProp = GetHiararchyComponentFromTag("prop");
	mProp->SetRotationZ(mProp->GetLocalRotation().z + 100 * m_fSpeed);

	HierarchialComponent* mTurret = GetHiararchyComponentFromTag("turret");

	// Tilt gun up and down as turret rotates
	HierarchialComponent* mGun = GetHiararchyComponentFromTag("gun");

	if (!m_bGunCam)
	{
		mGun->SetLocalRotation(0, 0, 0);
		mTurret->SetLocalRotation(0, 0, 0);
	}
	else
	{
		mTurret->SetRotationY(mTurret->GetLocalRotation().y + 0.1f);
		mGun->SetRotationX(sin((float)XMConvertToRadians(mTurret->GetLocalRotation().y * 4.0f)) * 10.0f - 10.0f);
	}

	UpdateMatrices();

	// Move Forward
	XMFLOAT4 planePosition;
	XMVECTOR vCurrPos = XMLoadFloat4(&GetHiararchyComponentFromTag("plane")->GetLocalPosition());
	vCurrPos += m_vForwardVector * m_fSpeed;
	XMStoreFloat4(&planePosition, vCurrPos);

	GetHiararchyComponentFromTag("plane")->SetLocalPosition(planePosition);
}

void Aeroplane::Draw(XMFLOAT3 camPos)
{
	HierarchialParent::UpdateShader(camPos);
	DrawHierarchy();
}

//void Aeroplane::UpdateShader(XMFLOAT3 camPos)
//{
//	ID3D11DeviceContext* pContext = Application::s_pApp->GetDeviceContext();
//
//	if (m_pMyAppCBuffer)
//	{
//		D3D11_MAPPED_SUBRESOURCE map;
//		if (SUCCEEDED(pContext->Map(m_pMyAppCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map)))
//		{
//			// Set the buffer contents. There is only one variable to set in this case.
//			// This method relies on the offset which has been found through Shader Reflection.
//			SetCBufferFloat3(map, m_cameraPosOffset, D3DXVECTOR3(camPos.x, camPos.y, camPos.z));
//			pContext->Unmap(m_pMyAppCBuffer, 0);
//		}
//	}
//}

//bool Aeroplane::LoadShader()
//{
//
//	ID3D11VertexShader* pVS = NULL;
//	ID3D11PixelShader* pPS = NULL;
//	ID3D11InputLayout* pIL = NULL;
//
//	ShaderDescription vs, ps;
//
//	ID3D11Device* pDevice = Application::s_pApp->GetDevice();
//
//	char maxNumLightsValue[100];
//	_snprintf_s(maxNumLightsValue, sizeof maxNumLightsValue, _TRUNCATE, "%d", CommonApp::MAX_NUM_LIGHTS);
//
//	D3D_SHADER_MACRO aMacros[] = {
//		{
//			"MAX_NUM_LIGHTS",
//			maxNumLightsValue,
//		},
//		{ NULL },
//	};
//
//	if (!CompileShadersFromFile(pDevice, "./Resources/Shaders/SpecularShader.hlsl", "VSMain", &pVS, &vs, g_aVertexDesc_Pos3fColour4ubNormal3f,
//		g_vertexDescSize_Pos3fColour4ubNormal3f, &pIL, "PSMain", &pPS, &ps, aMacros))
//	{
//		return false; // false;
//	}
//
//	Application::s_pApp->CreateShaderFromCompiledShader(&m_shader, pVS, &vs, pIL, pPS, &ps);
//
//	ps.FindCBuffer("MyApp", &m_psMyAppCBufferSlot);
//	ps.FindFloat(m_psMyAppCBufferSlot, "g_cameraPosition", &m_cameraPosOffset);
//
//	// Create a cbuffer, using the shader description to find out how
//	// large it needs to be.
//	m_pMyAppCBuffer = CreateBuffer(pDevice, ps.GetCBufferSizeBytes(m_psMyAppCBufferSlot), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL);
//
//	// In this example we are sharing the constant buffer between both vertex and pixel shaders.
//	// This is efficient since we only update one buffer. However we could define separate constant buffers for each stage.
//	// Generally constant buffers should represent groups of variables that must be updated at the same rate.
//	// So : we might have 'per execution' 'per frame', 'per draw' constant buffers.
//
//	SetShaderForAll(&m_shader);
//}

//void Aeroplane::DeleteShader()
//{
//	HierarchialParent::DeleteShader();
//	Release(m_pMyAppCBuffer);
//
//	m_shader.Reset();
//}

void Aeroplane::UpdateMatrices()
{
	XMMATRIX mPlaneCameraRot, mForwardMatrix;

	UpdateHierarchy(false);
	UpdateCameraMatrix();

	m_vForwardVector = XMVector4Transform(XMVectorSet(0, 0, 1, 0), GetHiararchyComponentFromTag("plane")->GetWorldMatrix());
}

//TODO Cleanup by creating a camera component and adding to hierarchy
void Aeroplane::UpdateCameraMatrix()
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

void Aeroplane::UpdatePlaneMovement()
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

void Aeroplane::ResetMovementToZero()
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
