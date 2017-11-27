#pragma once

#include <vector>

#include "AnimationComponent.h"

//*********************************************************************************************
// File:			Animation.h
// Description:		Class which holds the complete hierarchy for one animation i.e A vector
//					of AnimationComponents. Each animation has it's own update timer
//					
// Notes:			
// Todo:			-Optimisation needs to be done.
//					-Start using a timer and delta time for updating local timer instead of 
//					 just a constant value
//					-Make blendTime settable per animation as it's hardcoded for now
//					-Make sure GetMaxTime() is only called once at the start instead of every
//					 frame
//					-
//*********************************************************************************************

class Animation
{
public:
	Animation();
	~Animation() = default;

	//To be called every frame for the animation to be updated
	void Update(bool mDebug);
	
	void SetTime(float mTime, float mMaxTime);
	float GetMaxTime();

	//Get an animation component based on it's name (name of node)
	AnimationComponent* GetAnimationComponentByName(char* componentName);

public:

	//Array of animation components for a hierarchy
	std::vector<AnimationComponent> m_vAnimationComponents;

public:
	void SetIsLoopable(bool mLoop) { m_bIsLoopable = mLoop; };
	bool GetIsLoopable() { return m_bIsLoopable; };

	void SetBlendTime(float mBlend) { m_fBlendTime = mBlend; };
	float GetBlendTime() { return m_fBlendTime; };

	float GetTimer() { return m_animTimer; };
	void ResetTimer() { m_animTimer = 0; };

private:
	void UpdateComponents();
	void UpdateTimer(bool mDebug);

private:

	//Name of animation
	std::string m_cAnimationName;

	//Counter for animation
	float m_animTimer;

	//Maximun time of animation
	float m_fMaxTime;

	//Time in which animation blends
	float m_fBlendTime;

	//Whether the animation is playing or not
	bool m_bIsPlaying;

	//Whether the animation should loop or not
	bool m_bIsLoopable;

	//Frame counter used for debug mode
	int m_iFrameCounter;
};

