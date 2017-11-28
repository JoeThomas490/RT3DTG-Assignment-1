#include "Robot.h"

Robot::Robot()
{
}

Robot::Robot(float fX, float fY, float fZ, float fRotY)
{
	HierarchialParent::HierarchialParent(XMFLOAT4(fX, fY, fZ, 0), XMFLOAT4(0, fRotY, 0, 0));


	LoadHierarchyFromFile("./Resources/Robot/hierarchy.txt", "Robot");

	GetHiararchyComponentFromTag("root")->SetLocalRotation(0.0f, fRotY, 0.0f);

	//ToDo Add colour to hierarchy.txt and also load that from file
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

void Robot::Update(bool mDebug)
{
	UpdateHierarchy(mDebug);
}

void Robot::Draw(XMFLOAT3 camPos, float mFrameCount)
{
	UpdateShader(camPos, mFrameCount);
	DrawHierarchy();
}
