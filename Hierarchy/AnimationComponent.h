#pragma once

#include "AnimationData.h"
#include "Application.h"

#include <string>

//*********************************************************************************************
// File:			AnimationComponent.h
// Description:		Holds all the animation data for a singular hierarchy component
//					e.g robot_pelvis
//							
// Notes:			
// Todo:			
//*********************************************************************************************

__declspec(align(16)) class AnimationComponent
{
public:


	AnimationComponent();
	~AnimationComponent();

	void Update(float mTime);

	//Array holding all 6 animations
	std::vector<AnimationData> m_animationData;

private:

	//Interpolate all animation data for the given time 
	//and place in current position/rotation
	void InterpolateTranslationData(float mTime);
	void InterpolateRotationData(float mTime);

	//Current position of animation pose
	XMFLOAT4 m_v4CurrentPos;
	//Current rotation of animation pose
	XMFLOAT4 m_v4CurrentRotation;

	//Name of node attached to this animation data
	std::string m_nodeName;

public:

	//Get/Set the node name associated with this component
	void SetNodeName(std::string nodeName) { m_nodeName = nodeName; };
	std::string* GetNodeName() { return &m_nodeName; };

	XMFLOAT4 GetCurrentPosition() { return m_v4CurrentPos; };
	XMFLOAT4 GetCurrentRotation() { return m_v4CurrentRotation; };
};

