#pragma once

#include <vector>

#include "AnimationComponent.h"

//*********************************************************************************************
// File:			Animation.h
// Description:		Class which holds the complete hierarchy for one animation i.e A vector
//					of AnimationComponents. Each animation has it's own update timer
//					
// Notes:			
// Todo:			
//*********************************************************************************************

class Animation
{
public:
	Animation();
	~Animation() = default;

	//To be called every frame for the animation to be updated
	void Update();

	void ResetTimer();

	//Get an animation component based on it's name (name of node)
	AnimationComponent* GetAnimationComponentByName(char* componentName);

public:

	//Array of animation components for a hierarchy
	std::vector<AnimationComponent> m_vAnimationComponents;

public:
	void SetIsLoopable(bool mLoop) { m_bIsLoopable = mLoop; };
	bool GetIsLoopable() { return m_bIsLoopable; };

	void SetDebugAnim(bool mDebug) { m_bDebugAnim = mDebug; };

private:

	void UpdateComponents();

	float GetMaxTime();

private:

	//Name of animation
	std::string m_cAnimationName;

	//Counter for animation
	float m_animTimer;

	//Maximun time of animation
	float m_fMaxTime;

	//Whether the animation is playing or not
	bool m_bIsPlaying;

	//Whether the animation should loop or not
	bool m_bIsLoopable;

	bool m_bDebugAnim;
};

