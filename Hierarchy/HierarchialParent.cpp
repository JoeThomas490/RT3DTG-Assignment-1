#include "HierarchialParent.h"



HierarchialParent::HierarchialParent()
{
	m_v4LocalPos = XMFLOAT4(0, 0, 0, 0);
	m_v4LocalRot = XMFLOAT4(0, 0, 0, 0);
}

HierarchialParent::HierarchialParent(XMFLOAT4 startPos, XMFLOAT4 startRot)
{
	m_v4LocalPos = startPos;
	m_v4LocalRot = startRot;
}

void HierarchialParent::AddComponent(HierarchialComponent * mComponents, int parentIndex)
{
	m_pHierarchyComponents.push_back(mComponents);
	m_componentParents.push_back(parentIndex);
}

void HierarchialParent::AddHierarchyComponent(HierarchialComponent * mComponent, char * tag)
{
	m_mHierarchyComponents.emplace(tag, mComponent);
}

HierarchialComponent * HierarchialParent::GetHiararchyComponentFromTag(char * tag)
{
	//If it can't be found in the map return a nullptr
	if (m_mHierarchyComponents.find(tag) == m_mHierarchyComponents.end())
	{
		return nullptr;
	}
	//Otherwise return the pointer
	else
	{
		return m_mHierarchyComponents.at(tag);
	}
}

void HierarchialParent::UpdateHierarchy()
{
	CalculateLocalMatrices();
	CalculateWorldMatrices();
}

void HierarchialParent::DrawHierarchy()
{
	for (const auto& component : m_pHierarchyComponents)
	{
		component->Draw();
	}
}

void HierarchialParent::CalculateLocalMatrices()
{
	for (const auto& component : m_pHierarchyComponents)
	{
		component->UpdateLocalMatrix();
	}
}

void HierarchialParent::CalculateWorldMatrices()
{
	int currentComponentInd = 0;

	//Loop through each component
	for (auto &component : m_pHierarchyComponents)
	{
		//If the component has a parent
		if (m_componentParents[currentComponentInd] != -1)
		{
			int parentInd = m_componentParents[currentComponentInd];
			XMMATRIX mWorldMatrix = component->GetLocalMatrix() * (m_pHierarchyComponents.at(parentInd)->GetWorldMatrix());
			component->SetWorldMatrix(&mWorldMatrix);
		}
		//If the component is the top parent
		else
		{
			component->SetWorldMatrix(&component->GetLocalMatrix());
		}

		currentComponentInd++;
	}
}
