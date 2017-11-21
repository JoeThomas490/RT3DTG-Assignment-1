#pragma once

#include <vector>

#include "AnimationComponent.h"

//*********************************************************************************************
// File:			Animation.h
// Description:		Class which holds the complete hierarchy for one animation.
//					
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

	//Get an animation component based on it's name (name of node)
	AnimationComponent* GetAnimationComponentByName(char* componentName);

public:

	//Array of animation components for a hierarchy
	std::vector<AnimationComponent> m_vAnimationComponents;

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
};

