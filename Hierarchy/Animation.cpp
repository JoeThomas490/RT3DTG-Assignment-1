#include "Animation.h"


const float ANIMATION_TICK = 0.0133f;


Animation::Animation()
{
	m_animTimer = 0;
	m_fMaxTime = 0;
}

void Animation::Update()
{
	//Increment animation timer
	m_animTimer += ANIMATION_TICK;

	//If the timer has gone past the max time
	if (m_animTimer > GetMaxTime())
	{
		//Reset the timer
		m_animTimer = 0;
	}

	UpdateComponents();
}

void Animation::UpdateComponents()
{
	//Loop through all the components in the animation hierarchy
	for (auto& component : m_vAnimationComponents)
	{
		//Update the component, passing it the current time
		component.Update(m_animTimer);
	}
}

AnimationComponent * Animation::GetAnimationComponentByName(char * componentName)
{
	//Loop through every component
	for (int i = 0; i < m_vAnimationComponents.size(); i++)
	{
		//If the names match each other
		if (m_vAnimationComponents[i].GetNodeName() == componentName)
		{
			//Return a pointer to that component
			return &m_vAnimationComponents[i];
		}
	}

	//Otherwise return a nullptr
	//TODO Change this as it's not good to return a null ptr
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


