#include "Robot.h"

CommonMesh* Robot::s_pBodyMesh = nullptr;
CommonMesh* Robot::s_pLeftAnkleMesh = nullptr;
CommonMesh* Robot::s_pLeftElbowMesh = nullptr;
CommonMesh* Robot::s_pLeftHipMesh = nullptr;
CommonMesh* Robot::s_pLeftKneeMesh = nullptr;
CommonMesh* Robot::s_pLeftShoulderMesh = nullptr;
CommonMesh* Robot::s_pLeftWristMesh = nullptr;
CommonMesh* Robot::s_pNeckMesh = nullptr;
CommonMesh* Robot::s_pPelvisMesh = nullptr;
CommonMesh* Robot::s_pRightAnkleMesh = nullptr;
CommonMesh* Robot::s_pRightElbowMesh = nullptr;
CommonMesh* Robot::s_pRightHipMesh = nullptr;
CommonMesh* Robot::s_pRightKneeMesh = nullptr;
CommonMesh* Robot::s_pRightShoulderMesh = nullptr;
CommonMesh* Robot::s_pRightWristMesh = nullptr;

Robot::Robot()
{
}

Robot::Robot(float fX, float fY, float fZ, float fRotY)
{
	AddComponent(&m_hRoot, -1);						//0
	AddComponent(&m_hLeftHipComponent, 0);			//2
	AddComponent(&m_hRightHipComponent, 0);			//3
	AddComponent(&m_hLeftKneeComponent, 1);			//5
	AddComponent(&m_hRightKneeComponent, 2);		//6
	AddComponent(&m_hLeftAnkleComponent, 3);		//7
	AddComponent(&m_hRightAnkleComponent,4);		//8
	AddComponent(&m_hPelvisComponent, 0);			//1
	AddComponent(&m_hBodyComponent, 7);				//4
	AddComponent(&m_hNeckComponent, 8);				//11
	AddComponent(&m_hLeftShoulderComponent, 8);		//9
	AddComponent(&m_hRightShoulderComponent, 8);	//10
	AddComponent(&m_hLeftElbowComponent, 10);		//12
	AddComponent(&m_hRightElbowComponent, 11);		//13
	AddComponent(&m_hLeftWristComponent, 12);		//14
	AddComponent(&m_hRightWristComponent, 13);		//15


	m_hRoot.SetLocalPosition(fX + (1.027778f / 10.0f), fY + (75.644722f / 10.0f), fZ);
	m_hRoot.SetLocalRotation(0.0f, fRotY, 0.0f);

	m_hPelvisComponent.SetLocalPosition((-0.250011 / 10.0f), (15.250000f / 10.0f), (-0.00005f / 10.0f));
	
	m_hBodyComponent.SetLocalPosition((0.500099f / 10.0f), (43.749992f / 10.0f), (0.000003f / 10.0f));

	m_hLeftShoulderComponent.SetLocalPosition((46.0f / 10.0f), 0.0f, (-0.009992f / 10.0f));
	m_hLeftElbowComponent.SetLocalPosition((34.250019f / 10.0f), (-0.499817f / 10.0f), (-0.004262f / 10.0f));
	m_hLeftWristComponent.SetLocalPosition((55.250008f / 10.0f), (-0.999710f / 10.0f), (0.003968f / 10.0f));

	m_hRightShoulderComponent.SetLocalPosition((-44.500023f / 10.0f), (0.5f / 10.0f), (-0.000021f / 10.0f));
	m_hRightElbowComponent.SetLocalPosition((-33.999996f / 10.0f), (0.250229f / 10.0f), (-0.000194f / 10.0f));
	m_hRightWristComponent.SetLocalPosition((-60.000381f / 10.0f), (-1.750183f / 10.0f), (0.007156f / 10.0f));

	m_hNeckComponent.SetLocalPosition((0.249983f / 10.0f), (36.625015f / 10.0f), (25.999998f / 10.0f));

	m_hLeftHipComponent.SetLocalPosition((19.5f / 10.0f), (-7.724991f / 10.0f), 0.0f);
	m_hLeftKneeComponent.SetLocalPosition((0.000006f / 10.0f), (-22.200001f / 10.0f), 0.0f);
	m_hLeftAnkleComponent.SetLocalPosition((-0.800152f / 10.0f), (-36.399994f / 10.0f), (-0.000098f / 10.0f));

	m_hRightHipComponent.SetLocalPosition((-19.5f / 10.0f), (-7.724991f / 10.0f), 0.0f);
	m_hRightKneeComponent.SetLocalPosition((0.000006f / 10.0f), (-22.0f / 10.0f), 0.0f);
	m_hRightAnkleComponent.SetLocalPosition((0.199911f / 10.0f), (-36.7999995f / 10.0f), (0.000039f / 10.0f));


}


Robot::~Robot()
{
}

void Robot::LoadResources()
{
	s_pPelvisMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/pelvis.x");
	s_pBodyMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/body.x");

	s_pLeftShoulderMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/left_shoulder.x");
	s_pLeftElbowMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/left_elbow.x");
	s_pLeftWristMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/left_wrist.x");

	s_pRightShoulderMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/right_shoulder.x");
	s_pRightElbowMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/right_elbow.x");
	s_pRightWristMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/right_wrist.x");

	s_pNeckMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/neck.x");

	s_pLeftHipMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/left_hip.x");
	s_pLeftKneeMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/left_knee.x");
	s_pLeftAnkleMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/left_ankle.x");

	s_pRightHipMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/right_hip.x");
	s_pRightKneeMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/right_knee.x");
	s_pRightAnkleMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Robot/right_ankle.x");

}	

void Robot::ReleaseResources()
{
	delete s_pPelvisMesh;
	delete s_pBodyMesh;

	delete s_pLeftShoulderMesh;
	delete s_pLeftElbowMesh;
	delete s_pLeftWristMesh;

	delete s_pRightShoulderMesh;
	delete s_pRightElbowMesh;
	delete s_pRightWristMesh;

	delete s_pNeckMesh;

	delete s_pLeftHipMesh;
	delete s_pLeftKneeMesh;
	delete s_pLeftAnkleMesh;

	delete s_pRightHipMesh;
	delete s_pRightKneeMesh;
	delete s_pRightAnkleMesh;

}

void Robot::Update()
{
	UpdateHierarchy();
}

void Robot::Draw()
{
	m_hPelvisComponent.Draw(s_pPelvisMesh);
	m_hBodyComponent.Draw(s_pBodyMesh);

	m_hLeftShoulderComponent.Draw(s_pLeftShoulderMesh);
	m_hLeftElbowComponent.Draw(s_pLeftElbowMesh);
	m_hLeftWristComponent.Draw(s_pLeftWristMesh);

	m_hRightShoulderComponent.Draw(s_pRightShoulderMesh);
	m_hRightElbowComponent.Draw(s_pRightElbowMesh);
	m_hRightWristComponent.Draw(s_pRightWristMesh);

	m_hNeckComponent.Draw(s_pNeckMesh);

	m_hLeftHipComponent.Draw(s_pLeftHipMesh);
	m_hLeftKneeComponent.Draw(s_pLeftKneeMesh);
	m_hLeftAnkleComponent.Draw(s_pLeftAnkleMesh);

	m_hRightHipComponent.Draw(s_pRightHipMesh);
	m_hRightKneeComponent.Draw(s_pRightKneeMesh);
	m_hRightAnkleComponent.Draw(s_pRightAnkleMesh);
}

void Robot::UpdateMatrices()
{
}
