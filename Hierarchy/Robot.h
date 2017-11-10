#pragma once

#include "Application.h"

#include "HierarchialParent.h"
#include "HierarchialComponent.h"

__declspec(align(16))class Robot : public HierarchialParent
{
public:
	Robot();
	Robot(float fX, float fY, float fZ, float fRotY);
	~Robot();

	static void LoadResources();
	static void ReleaseResources();

	void Update();
	void Draw();


private:
	void UpdateMatrices();

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



	static CommonMesh* s_pPelvisMesh;
	static CommonMesh* s_pBodyMesh;

	static CommonMesh* s_pLeftShoulderMesh;
	static CommonMesh* s_pLeftElbowMesh;
	static CommonMesh* s_pLeftWristMesh;

	static CommonMesh* s_pRightShoulderMesh;
	static CommonMesh* s_pRightElbowMesh;
	static CommonMesh* s_pRightWristMesh;

	static CommonMesh* s_pNeckMesh;

	static CommonMesh* s_pLeftHipMesh;
	static CommonMesh* s_pLeftKneeMesh;
	static CommonMesh* s_pLeftAnkleMesh;

	static CommonMesh* s_pRightHipMesh;
	static CommonMesh* s_pRightKneeMesh;
	static CommonMesh* s_pRightAnkleMesh;;
};

