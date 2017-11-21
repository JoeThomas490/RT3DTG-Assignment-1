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


void HierarchialParent::AddHierarchyComponent(HierarchialComponent * mComponent, char * tag)
{
	//Add the component into the map with it's associated tag
	m_mHierarchyComponents.emplace(tag, mComponent);

	//Add the tag to a vector to hold the order of insertion
	//TODO Think of another way to handle this as having a map and a vector is a bit overkill
	m_vHierarchyOrder.push_back(tag);
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
		return m_mHierarchyComponents.find(tag)->second;
	}
}

void HierarchialParent::UpdateHierarchy()
{

	m_animation.Update();

	for (std::map<char*, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
	{
		AnimationComponent* ac = m_animation.GetAnimationComponentByName(it->first);

		XMFLOAT4 animPosition = ac->GetCurrentPosition();

		it->second->SetLocalPosition(animPosition);
		it->second->SetLocalRotation(ac->GetCurrentRotation());
	}

	CalculateLocalMatrices();
	CalculateWorldMatrices();
}

void HierarchialParent::DrawHierarchy()
{
	for (std::map<char*, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
	{
		it->second->Draw();
	}
}

void HierarchialParent::SetOffsetForComponent(char * tag, XMFLOAT4 m_offset)
{
	AnimationComponent* ac = m_animation.GetAnimationComponentByName(tag);
	

	for (auto& animData : ac->m_animationData)
	{
		AnimationData::AnimationType animType = animData.GetAnimationType();

		switch (animType)
		{
		case AnimationData::TRANSLATE_X:
			for (int i = 0; i < animData.GetAnimationCount(); i++)
			{
				animData.AddOffset(i, m_offset.x);
			}
			break;
		case AnimationData::TRANSLATE_Y:
			for (int i = 0; i < animData.GetAnimationCount(); i++)
			{
				animData.AddOffset(i, m_offset.y);
			}
			break;
		case AnimationData::TRANSLATE_Z:
			for (int i = 0; i < animData.GetAnimationCount(); i++)
			{
				animData.AddOffset(i, m_offset.z);
			}
			break;
		}
	}


}

void HierarchialParent::CalculateLocalMatrices()
{
	for (std::map<char*, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
	{
		it->second->UpdateLocalMatrix();
	}
}

void HierarchialParent::CalculateWorldMatrices()
{
	for (auto& tag : m_vHierarchyOrder)
	{
		HierarchialComponent* hc = m_mHierarchyComponents.find(tag)->second;
		if (hc->GetParentNode() != "")
		{
			char* parentNode = hc->GetParentNode();
			XMMATRIX mWorldMatrix = hc->GetLocalMatrix() * (m_mHierarchyComponents.at(parentNode)->GetWorldMatrix());
			hc->SetWorldMatrix(&mWorldMatrix);
		}
		else
		{
			hc->SetWorldMatrix(&hc->GetLocalMatrix());
		}
	}
}
