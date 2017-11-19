#pragma once

#include <vector>

#include "AnimationComponent.h"

//*********************************************************************************************
// File:			Animation.h
// Description:		 
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

	void Update();

	void Play();

	AnimationComponent* GetAnimationComponentByName(char* componentName);

public:

	std::vector<AnimationComponent> m_vAnimationComponents;

private:

	float GetMaxTime();

private:

	std::string m_cAnimationName;

	float m_animTimer;
	float m_fMaxTime;

	bool m_bIsPlaying;
};

