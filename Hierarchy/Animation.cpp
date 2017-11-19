#include "Animation.h"



Animation::Animation()
{
	m_animTimer = 0;
	m_fMaxTime = 0;
}

void Animation::Update()
{
	m_animTimer += 0.0133f;

	if (m_animTimer > GetMaxTime())
	{
		m_animTimer = 0;
	}

	UpdateComponents();
}

void Animation::UpdateComponents()
{
	for (auto& component : m_vAnimationComponents)
	{
		component.Update(m_animTimer);
	}
}



AnimationComponent * Animation::GetAnimationComponentByName(char * componentName)
{
	for (int i = 0; i < m_vAnimationComponents.size(); i++)
	{
		if (m_vAnimationComponents[i].GetNodeName() == componentName)
		{
			return &m_vAnimationComponents[i];
		}
	}
	return nullptr;
}

float Animation::GetMaxTime()
{
	float maxTime = 0;

	for (auto& component : m_vAnimationComponents)
	{
		for (auto& data : component.m_animationData)
		{
			float thisMaxTime, a;
			data.GetTimeValuePair(data.GetAnimationCount() - 1, thisMaxTime, a);
			if (maxTime < thisMaxTime)
			{
				maxTime = thisMaxTime;
			}
		}
	}
	
	m_fMaxTime = maxTime;
	return maxTime;
}


