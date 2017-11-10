#pragma once

#include <vector>

#include "Application.h"
#include "HierarchialComponent.h"

class HierarchialParent
{
public:
	HierarchialParent();
	HierarchialParent(XMFLOAT4 startPos, XMFLOAT4 startRot);
	
	void AddComponent(HierarchialComponent* mComponents, int parentIndex);

	void UpdateHierarchy();

	void DrawHierarchy();

	~HierarchialParent() = default;

private:
	std::vector<HierarchialComponent*> m_pHierarchyComponents;
	std::vector<int> m_componentParents;

	void CalculateLocalMatrices();
	void CalculateWorldMatrices();

	XMFLOAT4 m_v4LocalPos;
	XMFLOAT4 m_v4LocalRot;

	XMFLOAT4 m_v4LocalScale;

};

