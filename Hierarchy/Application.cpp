#include "Application.h"

#include "Heightmap.h"
#include "Aeroplane.h"
#include "Bullet.h"
#include "Robot.h"
#include "MeshManager.h"
#include "AnimationLoader.h"

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
	m_bDebugAnimations = false;

	m_pHeightMap = new HeightMap("Resources/heightmap.bmp", 2.0f);
	m_pAeroplane = new Aeroplane(0.0f, 6.5f, 10.0f, 105.0f);

	Robot::LoadResources();
	m_pRobot = new Robot(10.0f, 10.0f, -20.0f, 0.0f);

	for (int i = 0; i < 5; i++)
	{
		m_pRobots.push_back(new Robot(cos(i * 3.14f) * 10.0f, 10.0f, sin(i * 3.14f) * 10.0f, i * 25.0f));
	}


	Bullet::LoadResources();

	m_cameraZ = 50.0f;
	m_rotationAngle = 0.f;

	if (!this->CommonApp::HandleStart())
		return false;

	this->SetRasterizerState(false, m_bWireframe);

	m_cameraState = CAMERA_MAP;

	LoadXML();

	m_pRobot->SetLocalPosition(0.0f, 2.5f, 10.0f);

	int index = 0;
	for (int i = 0; i < 360; i+= 360/5)
	{
		m_pRobots[index]->SetLocalPosition(cos(XMConvertToRadians(i)) * 25.0f, 2.0f, sin(XMConvertToRadians(i)) * 25.0f);
		m_pRobots[index]->SetLocalRotation(0, 360 - (i), 0);
		index++;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleStop()
{
	delete m_pHeightMap;

	delete m_pAeroplane;

	Bullet::ReleaseResources();

	//Robot::ReleaseResources();
	delete m_pRobot;
	m_pRobot = nullptr;

	for (auto& robot : m_pRobots)
	{
		delete robot;
	}

	this->CommonApp::HandleStop();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleUpdate()
{

	for (auto& robot : m_pRobots)
	{
		robot->Update();
	}

	m_rotationAngle += .01f;

	if (m_cameraState == CAMERA_MAP)
	{
		if (this->IsKeyPressed('Q'))
			m_cameraZ -= 2.0f;

		if (this->IsKeyPressed('A'))
			m_cameraZ += 2.0f;
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

	static AnimationLoader m_animLoader;

	static bool db1 = false;
	if (this->IsKeyPressed('1'))
	{
		if (!db1)
		{
			for (auto& robot : m_pRobots)
			{
				robot->SetBlendingAnimation(robot->GetAnimation(0));
				//robot->GetActiveAnimation()->ResetTimer();
			}
			
			db1 = true;
		}
	}
	else
	{
		db1 = false;
	}

	static bool db2 = false;
	if (this->IsKeyPressed('2'))
	{
		if (!db2)
		{
			for (auto& robot : m_pRobots)
			{
				robot->SetBlendingAnimation(robot->GetAnimation(1));
			}
			db2 = true;
		}
	}
	else
	{
		db2 = false;
	}

	static bool db3 = false;
	if (this->IsKeyPressed('3'))
	{
		if (!db3)
		{
			for (auto& robot : m_pRobots)
			{
				robot->SetBlendingAnimation(robot->GetAnimation(2));
			}
			db3 = true;
		}
	}
	else
	{
		db3 = false;
	}

	if (this->IsKeyPressed('F'))
	{
		m_bDebugAnimations = true;
	}
	else
	{
		m_bDebugAnimations = false;
	}


	m_pAeroplane->Update(m_cameraState != CAMERA_MAP);

	static bool dbSpace = false;

	//If space bar is pressed
	if (this->IsKeyPressed(32))
	{
		if (!dbSpace)
		{
			dbSpace = true;

			XMMATRIX mGunWorldMatrix = m_pAeroplane->GetGunWorldMatrix();
			XMFLOAT4 mPlaneForwardVector = m_pAeroplane->GetForwardVector();

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
		m_pAeroplane->SetGunCamera(false);
		vCamera = XMFLOAT3(m_pAeroplane->GetCameraPosition().x, m_pAeroplane->GetCameraPosition().y, m_pAeroplane->GetCameraPosition().z);
		vLookat = XMFLOAT3(m_pAeroplane->GetFocusPosition().x, m_pAeroplane->GetFocusPosition().y, m_pAeroplane->GetFocusPosition().z);
		break;
	case CAMERA_GUN:
		m_pAeroplane->SetGunCamera(true);
		vCamera = XMFLOAT3(m_pAeroplane->GetCameraPosition().x, m_pAeroplane->GetCameraPosition().y, m_pAeroplane->GetCameraPosition().z);
		vLookat = XMFLOAT3(m_pAeroplane->GetFocusPosition().x, m_pAeroplane->GetFocusPosition().y, m_pAeroplane->GetFocusPosition().z);
		break;
	case CAMERA_ROBOT:
		m_pAeroplane->SetGunCamera(false);
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

	m_pAeroplane->Draw();

	for (auto& robot : m_pRobots)
	{
		robot->Draw();
	}

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		m_arrBullets[i].Draw();
	}
}


void Application::LoadXML()
{
	AnimationLoader loader;
	for (auto& robot : m_pRobots)
	{
		robot->AddAnimation(loader.LoadXML("Resources/Robot/Animations/RobotDieAnimDAE.xml"));
		robot->AddAnimation(loader.LoadXML("Resources/Robot/Animations/RobotIdleAnimDAE.xml"));
		robot->AddAnimation(loader.LoadXML("Resources/Robot/Animations/RobotAttackAnimDAE.xml"));

		robot->SetActiveAnimation(0);
		robot->GetActiveAnimation()->SetIsLoopable(false);
	}
	/*m_pRobot->AddAnimation(loader.LoadXML("Resources/Robot/Animations/RobotDieAnimDAE.xml"));
	m_pRobot->AddAnimation(loader.LoadXML("Resources/Robot/Animations/RobotIdleAnimDAE.xml"));
	m_pRobot->AddAnimation(loader.LoadXML("Resources/Robot/Animations/RobotAttackAnimDAE.xml"));*/

	//m_pRobot->SetActiveAnimation(0);
	//m_pRobot->GetActiveAnimation()->SetIsLoopable(false);
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
