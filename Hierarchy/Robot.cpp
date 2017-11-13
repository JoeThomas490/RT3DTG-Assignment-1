#include "Robot.h"

Robot::Robot()
{
}

Robot::Robot(float fX, float fY, float fZ, float fRotY)
{
	AddComponents();
	
	m_hPelvisComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/pelvis.x", "robot_pelvis"));
	m_hBodyComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/body.x", "robot_body"));

	m_hLeftShoulderComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/left_shoulder.x", "robot_left_shoulder"));
	m_hLeftElbowComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/left_elbow.x", "robot_left_elbow"));
	m_hLeftWristComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/left_wrist.x", "robot_left_wrist"));

	m_hRightShoulderComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/right_shoulder.x", "robot_right_shoulder"));
	m_hRightElbowComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/right_elbow.x", "robot_right_elbow"));
	m_hRightWristComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/right_wrist.x", "robot_right_wrist"));

	m_hNeckComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/neck.x", "robot_neck"));

	m_hLeftHipComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/left_hip.x", "robot_left_hip"));
	m_hLeftKneeComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/left_knee.x", "robot_left_knee"));
	m_hLeftAnkleComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/left_ankle.x", "robot_left_ankle"));

	m_hRightHipComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/right_hip.x", "robot_right_hip"));
	m_hRightKneeComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/right_knee.x", "robot_right_knee"));
	m_hRightAnkleComponent.SetMesh(MeshManager::GetInstance().LoadResources("Resources/Robot/right_ankle.x", "robot_right_ankle"));

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

void Robot::AddComponents()
{

	//TODO Remove this for loading in the offset and component data from a hierarchy text file

	AddComponent(&m_hRoot, -1);
	AddComponent(&m_hLeftHipComponent, 0);
	AddComponent(&m_hRightHipComponent, 0);
	AddComponent(&m_hLeftKneeComponent, 1);
	AddComponent(&m_hRightKneeComponent, 2);
	AddComponent(&m_hLeftAnkleComponent, 3);
	AddComponent(&m_hRightAnkleComponent, 4);
	AddComponent(&m_hPelvisComponent, 0);
	AddComponent(&m_hBodyComponent, 7);
	AddComponent(&m_hNeckComponent, 8);
	AddComponent(&m_hLeftShoulderComponent, 8);
	AddComponent(&m_hRightShoulderComponent, 8);
	AddComponent(&m_hLeftElbowComponent, 10);
	AddComponent(&m_hRightElbowComponent, 11);
	AddComponent(&m_hLeftWristComponent, 12);
	AddComponent(&m_hRightWristComponent, 13);
}

void Robot::Update()
{
	UpdateHierarchy();
}

void Robot::Draw()
{
	DrawHierarchy();
}
