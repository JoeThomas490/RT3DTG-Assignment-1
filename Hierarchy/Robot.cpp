#include "Robot.h"

Robot::Robot()
{
}

Robot::Robot(float fX, float fY, float fZ, float fRotY)
{
	HierarchialParent(XMFLOAT4(fX, fY, fZ, 0), XMFLOAT4(0, fRotY, 0, 0));


	AddComponents();
	GetHiararchyComponentFromTag("root")->SetLocalRotation(0.0f, fRotY, 0.0f);

	GetHiararchyComponentFromTag("body")->SetColor(0.666f, 0.572f, 0.572f, 1.0f);
	GetHiararchyComponentFromTag("neck")->SetColor(0.933f, 0.905f, 0.905f, 1.0f);
	GetHiararchyComponentFromTag("pelvis")->SetColor(0.666f, 0.572f, 0.572f, 1.0f);

	GetHiararchyComponentFromTag("left_hip")->SetColor(0.584f, 0.494f, 0.494f, 1.0f);
	GetHiararchyComponentFromTag("right_hip")->SetColor(0.584f, 0.494f, 0.494f, 1.0f);
	GetHiararchyComponentFromTag("left_knee")->SetColor(0.933f, 0.905f, 0.905f, 1.0f);
	GetHiararchyComponentFromTag("right_knee")->SetColor(0.933f, 0.905f, 0.905f, 1.0f);
	GetHiararchyComponentFromTag("left_ankle")->SetColor(0.309f, 0.188f, 0.188f, 1.0f);
	GetHiararchyComponentFromTag("right_ankle")->SetColor(0.309f, 0.188f, 0.188f, 1.0f);
	
	GetHiararchyComponentFromTag("left_shoulder")->SetColor(0.584f, 0.494f, 0.494f, 1.0f);
	GetHiararchyComponentFromTag("right_shoulder")->SetColor(0.584f, 0.494f, 0.494f, 1.0f);
	GetHiararchyComponentFromTag("left_elbow")->SetColor(0.933f, 0.905f, 0.905f, 1.0f);
	GetHiararchyComponentFromTag("right_elbow")->SetColor(0.933f, 0.905f, 0.905f, 1.0f);
	GetHiararchyComponentFromTag("left_wrist")->SetColor(0.666f, 0.074f, 0.098f, 1.0f);
	GetHiararchyComponentFromTag("right_wrist")->SetColor(0.666f, 0.074f, 0.098f, 1.0f);

}

void Robot::LoadResources()
{
	MeshManager::GetInstance().LoadResources("Resources/Robot/left_hip.x", "robot_left_hip");
	MeshManager::GetInstance().LoadResources("Resources/Robot/right_hip.x", "robot_right_hip");
	MeshManager::GetInstance().LoadResources("Resources/Robot/left_knee.x", "robot_left_knee");
	MeshManager::GetInstance().LoadResources("Resources/Robot/right_knee.x", "robot_right_knee");
	MeshManager::GetInstance().LoadResources("Resources/Robot/left_ankle.x", "robot_left_ankle");
	MeshManager::GetInstance().LoadResources("Resources/Robot/right_ankle.x", "robot_right_ankle");
	MeshManager::GetInstance().LoadResources("Resources/Robot/body.x", "robot_body");
	MeshManager::GetInstance().LoadResources("Resources/Robot/pelvis.x", "robot_pelvis");
	MeshManager::GetInstance().LoadResources("Resources/Robot/neck.x", "robot_neck");
	MeshManager::GetInstance().LoadResources("Resources/Robot/left_shoulder.x", "robot_left_shoulder");
	MeshManager::GetInstance().LoadResources("Resources/Robot/right_shoulder.x", "robot_right_shoulder");
	MeshManager::GetInstance().LoadResources("Resources/Robot/left_elbow.x", "robot_left_elbow");
	MeshManager::GetInstance().LoadResources("Resources/Robot/right_elbow.x", "robot_right_elbow");
	MeshManager::GetInstance().LoadResources("Resources/Robot/left_wrist.x", "robot_left_wrist");
	MeshManager::GetInstance().LoadResources("Resources/Robot/right_wrist.x", "robot_right_wrist");
}

void Robot::AddComponents()
{

	//TODO Remove this for loading in the offset and component data from a hierarchy text file

	AddHierarchyComponent(new HierarchialComponent(""), "root");

	AddHierarchyComponent(new HierarchialComponent("root", MeshManager::GetInstance().GetMeshFromTag("robot_left_hip")), "left_hip");
	AddHierarchyComponent(new HierarchialComponent("root", MeshManager::GetInstance().GetMeshFromTag("robot_right_hip")), "right_hip");
	AddHierarchyComponent(new HierarchialComponent("left_hip", MeshManager::GetInstance().GetMeshFromTag("robot_left_knee")), "left_knee");
	AddHierarchyComponent(new HierarchialComponent("right_hip", MeshManager::GetInstance().GetMeshFromTag("robot_right_knee")), "right_knee");
	AddHierarchyComponent(new HierarchialComponent("left_knee", MeshManager::GetInstance().GetMeshFromTag("robot_left_ankle")), "left_ankle");
	AddHierarchyComponent(new HierarchialComponent("right_knee", MeshManager::GetInstance().GetMeshFromTag("robot_right_ankle")), "right_ankle");

	AddHierarchyComponent(new HierarchialComponent("root", MeshManager::GetInstance().GetMeshFromTag("robot_pelvis")), "pelvis");
	AddHierarchyComponent(new HierarchialComponent("pelvis", MeshManager::GetInstance().GetMeshFromTag("robot_body")), "body");
	AddHierarchyComponent(new HierarchialComponent("body", MeshManager::GetInstance().GetMeshFromTag("robot_neck")), "neck");

	AddHierarchyComponent(new HierarchialComponent("body", MeshManager::GetInstance().GetMeshFromTag("robot_left_shoulder")), "left_shoulder");
	AddHierarchyComponent(new HierarchialComponent("body", MeshManager::GetInstance().GetMeshFromTag("robot_right_shoulder")), "right_shoulder");
	AddHierarchyComponent(new HierarchialComponent("left_shoulder", MeshManager::GetInstance().GetMeshFromTag("robot_left_elbow")), "left_elbow");
	AddHierarchyComponent(new HierarchialComponent("right_shoulder", MeshManager::GetInstance().GetMeshFromTag("robot_right_elbow")), "right_elbow");
	AddHierarchyComponent(new HierarchialComponent("left_elbow", MeshManager::GetInstance().GetMeshFromTag("robot_left_wrist")), "left_wrist");
	AddHierarchyComponent(new HierarchialComponent("right_elbow", MeshManager::GetInstance().GetMeshFromTag("robot_right_wrist")), "right_wrist");
}

void Robot::Update(bool mDebug)
{
	UpdateHierarchy(mDebug);
}

void Robot::Draw(XMFLOAT3 camPos)
{
	//UpdateShader(camPos);
	DrawHierarchy();
}
