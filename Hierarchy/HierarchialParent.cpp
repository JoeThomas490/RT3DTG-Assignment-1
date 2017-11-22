#include "HierarchialParent.h"



HierarchialParent::HierarchialParent()
{
	m_v4LocalPos = XMFLOAT4(0, 0, 0, 0);
	m_v4LocalRot = XMFLOAT4(0, 0, 0, 0);

	m_pActiveAnimation = nullptr;
	m_pBlendingAnimation = nullptr;

	m_fBlendTimer = 0;
}

HierarchialParent::HierarchialParent(XMFLOAT4 startPos, XMFLOAT4 startRot)
{
	m_v4LocalPos = startPos;
	m_v4LocalRot = startRot;

	m_pActiveAnimation = nullptr;
	m_pBlendingAnimation = nullptr;

	m_fBlendTimer = 0;

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

	UpdateAnimations();

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
	m_pActiveAnimation = &(m_mAnimations[index]);
}

void HierarchialParent::SetBlendingAnimation(Animation * pBlend)
{
	m_pBlendingAnimation = pBlend;

	m_pBlendingAnimation->SetTime(m_pActiveAnimation->GetTimer(), m_pActiveAnimation->GetMaxTime());
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

void HierarchialParent::UpdateAnimations()
{
	//for (auto& anim : m_mAnimations)
	//{
	//	anim.Update();
	//}

	if (m_pActiveAnimation != nullptr)
	{
		m_pActiveAnimation->Update();

		if (m_pBlendingAnimation != NULL)
		{
			m_pBlendingAnimation->Update();


			if (m_fBlendTimer > m_pBlendingAnimation->GetBlendTime())
			{
				m_fBlendTimer = 0;
				m_pActiveAnimation = m_pBlendingAnimation;
				m_pBlendingAnimation = nullptr;
			}
		}


		for (std::map<char*, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
		{
			AnimationComponent* ac = m_pActiveAnimation->GetAnimationComponentByName(it->first);

			XMFLOAT4 activeAnimPosition = ac->GetCurrentPosition();
			XMFLOAT4 activeAnimRotation = ac->GetCurrentRotation();

			it->second->SetLocalPosition(activeAnimPosition);
			it->second->SetLocalRotationQuart(activeAnimRotation);

			//If the blending animation has been set
			if (m_pBlendingAnimation != NULL)
			{
				AnimationComponent* blendAnimCompo = m_pBlendingAnimation->GetAnimationComponentByName(it->first);

				XMVECTOR blendAnimPosition = XMLoadFloat4(&blendAnimCompo->GetCurrentPosition()) - XMLoadFloat4(&activeAnimPosition);
				XMVECTOR blendAnimRotation = XMLoadFloat4(&blendAnimCompo->GetCurrentRotation()) - XMLoadFloat4(&activeAnimRotation);

				float t = m_fBlendTimer / m_pBlendingAnimation->GetBlendTime();

				blendAnimPosition *= t;
				blendAnimRotation *= t;

				blendAnimPosition += XMLoadFloat4(&activeAnimPosition);
				blendAnimRotation += XMLoadFloat4(&activeAnimRotation);

				XMFLOAT4 finalPos, finalRot;
				XMStoreFloat4(&finalPos, blendAnimPosition);
				XMStoreFloat4(&finalRot, blendAnimRotation);

				it->second->SetLocalPosition(finalPos);
				it->second->SetLocalRotationQuart(finalRot);
			}
		}

		if (m_pBlendingAnimation != NULL)
			m_fBlendTimer += 0.0133f;

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
