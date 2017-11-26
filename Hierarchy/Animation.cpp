#include "Animation.h"

const float ANIMATION_TICK = 0.0133f;


Animation::Animation()
{
	m_animTimer = 0;
	m_fMaxTime = 0;
	m_fBlendTime = 0.3f;

	m_bIsLoopable = true;

	m_iFrameCounter = 0;
}

void Animation::Update(bool mDebug)
{

	if(!mDebug)
	{
		//Increment animation timer
		m_animTimer += ANIMATION_TICK;
	}
	else
	{
		//Do debug frame counter here 
		m_iFrameCounter++;
		if (m_iFrameCounter > 60)
		{
			m_animTimer += ANIMATION_TICK * 2;
			m_iFrameCounter = 0;
		}
	}



	if (!m_bIsLoopable)
	{
		if (m_animTimer > GetMaxTime() - 0.1f)
		{
			m_animTimer = GetMaxTime() - 0.1f;
		}
	}
	else

		//If the timer has gone past the max time
		if (m_animTimer > GetMaxTime())
		{
			//Reset the timer
			m_animTimer = 0;
		}

	UpdateComponents();
}

void Animation::ResetTimer()
{
	m_animTimer = 0;
}

void Animation::SetTime(float mTime, float mMaxTime)
{
	float percent = mTime / mMaxTime;

	if (percent > 1)
	{
		m_animTimer = m_fMaxTime * percent;
	}
	m_animTimer = mTime;
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
		if (*m_vAnimationComponents[i].GetNodeName() == componentName)
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


