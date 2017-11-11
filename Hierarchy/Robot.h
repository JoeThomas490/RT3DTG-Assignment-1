#pragma once

#include "Application.h"

#include "HierarchialParent.h"
#include "HierarchialComponent.h"

#include "MeshManager.h"

__declspec(align(16))class Robot : public HierarchialParent
{
public:
	Robot();
	Robot(float fX, float fY, float fZ, float fRotY);
	~Robot() = default;

	void Update();
	void Draw();


private:

	void AddComponents();

private:
	HierarchialComponent m_hRoot;

	HierarchialComponent m_hPelvisComponent;
	HierarchialComponent m_hBodyComponent;

	HierarchialComponent m_hLeftShoulderComponent;
	HierarchialComponent m_hLeftElbowComponent;
	HierarchialComponent m_hLeftWristComponent;

	HierarchialComponent m_hRightShoulderComponent;
	HierarchialComponent m_hRightElbowComponent;
	HierarchialComponent m_hRightWristComponent;

	HierarchialComponent m_hNeckComponent;

	HierarchialComponent m_hLeftHipComponent;
	HierarchialComponent m_hLeftKneeComponent;
	HierarchialComponent m_hLeftAnkleComponent;

	HierarchialComponent m_hRightHipComponent;
	HierarchialComponent m_hRightKneeComponent;
	HierarchialComponent m_hRightAnkleComponent;
};

