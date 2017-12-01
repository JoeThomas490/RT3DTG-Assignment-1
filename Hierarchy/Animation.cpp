#include "Animation.h"

const float ANIMATION_TICK = 0.0133f;


//*********************************************************************************************
//************                           Constructor/Destructor                ****************
//*********************************************************************************************

Animation::Animation()
{
	//Initialise member variables
	m_animTimer = 0;
	m_fMaxTime = 0;
	m_iFrameCounter = 0;
	m_bIsLoopable = true;

	//Set blend time to a fixed value
	//TODO Make this settable per animation
	m_fBlendTime = 0.3f;
}

Animation::~Animation()
{
}

//*********************************************************************************************
//************                           Update functions                      ****************
//*********************************************************************************************

void Animation::Update(bool mDebug)
{
	//Get the max time of the animation
	//TODO Make sure this happens once at the start instead of every frame
	GetMaxTime();

	//Update the animation timer
	UpdateTimer(mDebug);

	//If the animation isn't loopable
	if (!m_bIsLoopable)
	{
		//When the animation timer reaches the max time
		if (m_animTimer > m_fMaxTime - 0.1f)
		{
			//Keep the animation timer at this time
			m_animTimer = m_fMaxTime  - 0.1f;
		}
	}
	else
	{
		//If the timer has gone past the max time
		if (m_animTimer > m_fMaxTime)
		{
			//Reset the timer
			m_animTimer = 0;
		}
	}

		
	//Update each component within the animation
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

void Animation::UpdateTimer(bool mDebug)
{
	//If we're not in debug mode
	if (!mDebug)
	{
		//Increment animation timer like normals
		m_animTimer += ANIMATION_TICK;
	}
	else
	{
		//If we're in debug mode
		//Incremement the frame counter 
		m_iFrameCounter++;
		//After 60 frames
		if (m_iFrameCounter > 60)
		{
			//Increment the animation timer 
			m_animTimer += ANIMATION_TICK * 2;
			//Reset the frame counter
			m_iFrameCounter = 0;
		}
	}
}

//*********************************************************************************************
//************                           Getters/Setters                       ****************
//*********************************************************************************************

void Animation::SetTime(float mTime, float mMaxTime)
{
	float percent = mTime / mMaxTime;

	if (percent > 1)
	{
		m_animTimer = m_fMaxTime * percent;
	}
	m_animTimer = mTime;
}

AnimationComponent * Animation::GetAnimationComponentByName(const std::string& componentName)
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

	//Loop through all the components
	for (auto& component : m_vAnimationComponents)
	{
		//Loop through all the data within the component
		for (auto& data : component.m_animationData)
		{
			float thisMaxTime, a;
			//Get the latest time in the animation data
			data.GetTimeValuePair(data.GetKeyframeCount() - 1, thisMaxTime, a);
			//If this new time is greater than our max time
			if (maxTime < thisMaxTime)
			{
				//Set our max time to this new one
				maxTime = thisMaxTime;
			}
		}
	}

	//Set our member varaible to this max time for later use
	m_fMaxTime = maxTime;

	//Return the max time
	return maxTime;
}

//*********************************************************************************************
//************                           END                                   ****************
//*********************************************************************************************

