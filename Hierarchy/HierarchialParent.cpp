#include "HierarchialParent.h"



HierarchialParent::HierarchialParent()
{
	m_v4LocalPos = XMFLOAT4(0, 0, 0, 0);
	m_v4LocalRot = XMFLOAT4(0, 0, 0, 0);

	m_pAnimation = nullptr;
}

HierarchialParent::HierarchialParent(XMFLOAT4 startPos, XMFLOAT4 startRot)
{
	m_v4LocalPos = startPos;
	m_v4LocalRot = startRot;

	m_pAnimation = nullptr;
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
	if(m_pAnimation != nullptr)
	{
		m_pAnimation->Update();

		for (std::map<char*, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
		{
			AnimationComponent* ac = m_pAnimation->GetAnimationComponentByName(it->first);

			XMFLOAT4 animPosition = ac->GetCurrentPosition();

			it->second->SetLocalPosition(animPosition);
			it->second->SetLocalRotationQuart(ac->GetCurrentRotation());
		}
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

void HierarchialParent::SetActiveAnimation(int index)
{
	m_pAnimation = &(m_mAnimations[index]);
}

void HierarchialParent::CalculateLocalMatrices()
{
	for (std::map<char*, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
	{
		it->second->UpdateLocalMatrix();
	}

	//Todo add local rotation and scaling here 
	XMMATRIX mRot = XMMatrixRotationQuaternion(CalculateQuaternion(m_v4LocalRot));
	m_mLocalMatrix = mRot * XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4LocalPos));
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
			hc->SetWorldMatrix(&(hc->GetLocalMatrix() * m_mLocalMatrix));
		}
	}
}

XMVECTOR HierarchialParent::CalculateQuaternion(XMFLOAT4 mRot)
{
	//Convert stored angle into radians
	double radX = XMConvertToRadians(mRot.x);
	double radY = XMConvertToRadians(mRot.y);
	double radZ = XMConvertToRadians(mRot.z);

	//Calculate cos components
	double c1 = cos(radY / 2.0);
	double c2 = cos(radZ / 2.0);
	double c3 = cos(radX / 2.0);

	//Calculate sin components
	double s1 = sin(radY / 2.0);
	double s2 = sin(radZ / 2.0);
	double s3 = sin(radX / 2.0);

	//Calculate quaternion value using components
	double w = (c1 * c2 * c3) - (s1 * s2 * s3);
	double x = (s1 * s2 * c3) + (c1 * c2 * s3);
	double y = (s1 * c2 * c3) + (c1 * s2 * s3);
	double z = (c1 * s2 * c3) - (s1 * c2 * s3);

	//Return as an XMVECTOR
	return XMVectorSet(x, y, z, w);
}
