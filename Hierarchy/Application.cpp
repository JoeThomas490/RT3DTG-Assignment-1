#include "Application.h"
#include "Heightmap.h"
#include "Aeroplane.h"
#include "AeroplaneTest.h"
#include "Bullet.h"
#include "Robot.h"
#include "MeshManager.h"

Application* Application::s_pApp = nullptr;

const int CAMERA_MAP = 0;
const int CAMERA_PLANE = 1;
const int CAMERA_GUN = 2;
const int CAMERA_ROBOT = 3;
const int CAMERA_MAX = 4;

const int MAX_BULLETS = 50;

Bullet m_arrBullets[50];


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool Application::HandleStart()
{
	s_pApp = this;

	this->SetWindowTitle("Assignment 1");

	m_bWireframe = false;

	m_pHeightMap = new HeightMap("Resources/heightmap.bmp", 2.0f);
	m_pAeroplane = new Aeroplane(0.0f, 3.5f, 0.0f, 105.0f);

	m_pAeroplaneTest = new AeroplaneTest(0.0f, 6.5f, 0.0f, 105.0f);
	//m_pAeroplaneTest->LoadResources();

	Robot::LoadResources();
	m_pRobot = new Robot(0.0f, 2.0f, -20.0f, 0.0f);

	Bullet::LoadResources();

	m_cameraZ = 50.0f;
	m_rotationAngle = 0.f;

	if (!this->CommonApp::HandleStart())
		return false;

	this->SetRasterizerState(false, m_bWireframe);

	m_cameraState = CAMERA_MAP;

	return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleStop()
{
	delete m_pHeightMap;

	Aeroplane::ReleaseResources();
	delete m_pAeroplane;

	delete m_pAeroplaneTest;

	Bullet::ReleaseResources();

	//Robot::ReleaseResources();
	delete m_pRobot;
	m_pRobot = nullptr;

	this->CommonApp::HandleStop();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleUpdate()
{
	m_pRobot->Update();

	m_rotationAngle += .01f;

	if (m_cameraState == CAMERA_MAP)
	{
		if (this->IsKeyPressed('Q'))
			m_cameraZ -= 2.0f;

		if (this->IsKeyPressed('A'))
			m_cameraZ += 2.0f;
	}


	static bool dbS = false;

	if (this->IsKeyPressed('S'))
	{
		if (!dbS)
		{
			m_pAeroplane->m_canMove = !m_pAeroplane->m_canMove;

			dbS = true;
		}
	}
	else
	{
		dbS = false;
	}

	static bool dbC = false;

	if (this->IsKeyPressed('C'))
	{
		if (!dbC)
		{
			if (++m_cameraState == CAMERA_MAX)
				m_cameraState = CAMERA_MAP;

			dbC = true;
		}
	}
	else
	{
		dbC = false;
	}

	static bool dbW = false;
	if (this->IsKeyPressed('W'))
	{
		if (!dbW)
		{
			m_bWireframe = !m_bWireframe;
			this->SetRasterizerState(false, m_bWireframe);
			dbW = true;
		}
	}
	else
	{
		dbW = false;
	}


	//m_pAeroplane->Update(m_cameraState != CAMERA_MAP);

	m_pAeroplaneTest->Update(m_cameraState != CAMERA_MAP);


	static bool dbSpace = false;

	//If space bar is pressed
	if (this->IsKeyPressed(32))
	{
		if (!dbSpace)
		{
			dbSpace = true;

			XMMATRIX mGunWorldMatrix = m_pAeroplaneTest->GetGunWorldMatrix();
			XMFLOAT4 mPlaneForwardVector = m_pAeroplaneTest->GetForwardVector();

			int freeIndex = 0;
			while (freeIndex < MAX_BULLETS)
			{
				if (!m_arrBullets[freeIndex].GetIsVisible())
				{
					break;
				}
				freeIndex++;
			}
			//Pass position and rotation

			m_arrBullets[freeIndex].ResetBullet(mGunWorldMatrix, mPlaneForwardVector, 1.0f);
		}
	}
	else
	{
		dbSpace = false;
	}


	for (int i = 0; i < MAX_BULLETS; i++)
	{
		m_arrBullets[i].Update();
	}
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleRender()
{
	XMFLOAT3 vUpVector(0.0f, 1.0f, 0.0f);
	XMFLOAT3 vCamera, vLookat;

	switch (m_cameraState)
	{
	case CAMERA_MAP:
		vCamera = XMFLOAT3(sin(m_rotationAngle) * m_cameraZ, m_cameraZ / 4, cos(m_rotationAngle) * m_cameraZ);
		vLookat = XMFLOAT3(0.0f, 4.0f, 0.0f);
		break;
	case CAMERA_PLANE:
		m_pAeroplaneTest->SetGunCamera(false);
		vCamera = XMFLOAT3(m_pAeroplaneTest->GetCameraPosition().x, m_pAeroplaneTest->GetCameraPosition().y, m_pAeroplaneTest->GetCameraPosition().z);
		vLookat = XMFLOAT3(m_pAeroplaneTest->GetFocusPosition().x, m_pAeroplaneTest->GetFocusPosition().y, m_pAeroplaneTest->GetFocusPosition().z);
		break;
	case CAMERA_GUN:
		m_pAeroplaneTest->SetGunCamera(true);
		vCamera = XMFLOAT3(m_pAeroplaneTest->GetCameraPosition().x, m_pAeroplaneTest->GetCameraPosition().y, m_pAeroplaneTest->GetCameraPosition().z);
		vLookat = XMFLOAT3(m_pAeroplaneTest->GetFocusPosition().x, m_pAeroplaneTest->GetFocusPosition().y, m_pAeroplaneTest->GetFocusPosition().z);
		break;
	case CAMERA_ROBOT:
		m_pAeroplaneTest->SetGunCamera(false);
		vCamera = XMFLOAT3(sin(12.1f) * m_cameraZ, m_cameraZ / 4, cos(12.1f) * m_cameraZ);
		vLookat = XMFLOAT3(0.0f, 5.0f, -20.0f);
		break;
	}

	XMMATRIX matView;
	matView = XMMatrixLookAtLH(XMLoadFloat3(&vCamera), XMLoadFloat3(&vLookat), XMLoadFloat3(&vUpVector));

	XMMATRIX matProj;
	matProj = XMMatrixPerspectiveFovLH(float(XM_PI / 4), 2, 1.5f, 5000.0f);

	this->SetViewMatrix(matView);
	this->SetProjectionMatrix(matProj);

	this->EnablePointLight(0, XMFLOAT3(100.0f, 100.f, -100.f), XMFLOAT3(1.f, 1.f, 1.f));
	this->SetLightAttenuation(0, 200.f, 2.f, 2.f, 2.f);
	this->EnableDirectionalLight(1, D3DXVECTOR3(-1.f, -1.f, -1.f), D3DXVECTOR3(0.65f, 0.55f, 0.65f));

	this->Clear(XMFLOAT4(.2f, .2f, .6f, 1.f));

	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	this->SetWorldMatrix(matWorld);

	m_pHeightMap->Draw();

	m_pAeroplaneTest->Draw();

	m_pRobot->Draw();

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		m_arrBullets[i].Draw();
	}
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application application;

	Run(&application);

	//Delete the mesh manager singleton
	delete &MeshManager::GetInstance();

	return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
