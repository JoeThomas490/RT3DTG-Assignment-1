#include "Robot.h"

Robot::Robot()
{
}

Robot::Robot(float fX, float fY, float fZ, float fRotY)
{
	HierarchialParent(XMFLOAT4(fX, fY, fZ, 0), XMFLOAT4(0, fRotY, 0, 0));


	AddComponents();
	GetHiararchyComponentFromTag("root")->SetLocalRotation(0.0f, fRotY, 0.0f);
	//GetHiararchyComponentFromTag("root")->SetLocalPosition(fX + (1.027778f / 10.0f), fY + (75.644722f / 10.0f), fZ);
	/*GetHiararchyComponentFromTag("pelvis")->SetLocalPosition((-0.250011 / 10.0f), (15.250000f / 10.0f), (-0.00005f / 10.0f));

	GetHiararchyComponentFromTag("body")->SetLocalPosition((0.500099f / 10.0f), (43.749992f / 10.0f), (0.000003f / 10.0f));

	GetHiararchyComponentFromTag("left_shoulder")->SetLocalPosition((46.0f / 10.0f), 0.0f, (-0.009992f / 10.0f));
	GetHiararchyComponentFromTag("left_elbow")->SetLocalPosition((34.250019f / 10.0f), (-0.499817f / 10.0f), (-0.004262f / 10.0f));
	GetHiararchyComponentFromTag("left_wrist")->SetLocalPosition((55.250008f / 10.0f), (-0.999710f / 10.0f), (0.003968f / 10.0f));

	GetHiararchyComponentFromTag("right_shoulder")->SetLocalPosition((-44.500023f / 10.0f), (0.5f / 10.0f), (-0.000021f / 10.0f));
	GetHiararchyComponentFromTag("right_elbow")->SetLocalPosition((-33.999996f / 10.0f), (0.250229f / 10.0f), (-0.000194f / 10.0f));
	GetHiararchyComponentFromTag("right_wrist")->SetLocalPosition((-60.000381f / 10.0f), (-1.750183f / 10.0f), (0.007156f / 10.0f));
*/
	//GetHiararchyComponentFromTag("neck")->SetLocalPosition((0.249983f / 10.0f), (36.625015f / 10.0f), (25.999998f / 10.0f));

	//GetHiararchyComponentFromTag("left_hip")->SetLocalPosition((19.5f / 10.0f), (-7.724991f / 10.0f), 0.0f);
	//GetHiararchyComponentFromTag("left_knee")->SetLocalPosition((0.000006f / 10.0f), (-22.200001f / 10.0f), 0.0f);
	//GetHiararchyComponentFromTag("left_ankle")->SetLocalPosition((-0.800152f / 10.0f), (-36.399994f / 10.0f), (-0.000098f / 10.0f));

	//GetHiararchyComponentFromTag("right_hip")->SetLocalPosition((-19.5f / 10.0f), (-7.724991f / 10.0f), 0.0f);
	//GetHiararchyComponentFromTag("right_knee")->SetLocalPosition((0.000006f / 10.0f), (-22.0f / 10.0f), 0.0f);
	//GetHiararchyComponentFromTag("right_ankle")->SetLocalPosition((0.199911f / 10.0f), (-36.7999995f / 10.0f), (0.000039f / 10.0f));
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

void Robot::Update()
{
	UpdateHierarchy();
}

void Robot::Draw()
{
	DrawHierarchy();
}
