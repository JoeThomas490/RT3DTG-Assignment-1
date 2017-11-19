#pragma once

#include "AnimationData.h"
#include <string>

//*********************************************************************************************
// File:			AnimationComponent.h
// Description:		 
//					
//					
// Notes:			
// Todo:			
//*********************************************************************************************

class AnimationComponent
{
public:
	AnimationComponent();
	~AnimationComponent() = default;

	AnimationData* GetDataByType(AnimationData::AnimationType animationType);

	//void AddData(AnimationData data);

	//Array holding all 6 animations
	std::vector<AnimationData> m_animationData;

private:

	

	//Name of node attached to this animation data
	std::string m_nodeName;

public:
	void SetNodeName(std::string nodeName) { m_nodeName = nodeName; };
	std::string GetNodeName() { return m_nodeName; };
};

