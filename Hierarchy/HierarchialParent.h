#pragma once

#include <vector>
#include <map>

#include "Application.h"
#include "HierarchialComponent.h"

class HierarchialParent
{
public:
	HierarchialParent();
	HierarchialParent(XMFLOAT4 startPos, XMFLOAT4 startRot);
	
	~HierarchialParent() = default;

	void AddComponent(HierarchialComponent* mComponents, int parentIndex);

	void AddHierarchyComponent(HierarchialComponent* mComponent, char* tag);

	HierarchialComponent* GetHiararchyComponentFromTag(char* tag);

	void UpdateHierarchy();

	void DrawHierarchy();


private:
	std::vector<HierarchialComponent*> m_pHierarchyComponents;
	std::vector<int> m_componentParents;

	std::map<char*, HierarchialComponent*> m_mHierarchyComponents;

	void CalculateLocalMatrices();
	void CalculateWorldMatrices();

	XMFLOAT4 m_v4LocalPos;
	XMFLOAT4 m_v4LocalRot;

	XMFLOAT4 m_v4LocalScale;

};

