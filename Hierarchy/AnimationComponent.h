#pragma once

#include "AnimationData.h"
#include "Application.h"

#include <string>

//*********************************************************************************************
// File:			AnimationComponent.h
// Description:		 
//					
//					
// Notes:			
// Todo:			
//*********************************************************************************************

__declspec(align(16)) class AnimationComponent
{
public:
	AnimationComponent();
	~AnimationComponent() = default;

	AnimationData* GetDataByType(AnimationData::AnimationType animationType);

	void Update(float mTime);

	//void AddData(AnimationData data);

	//Array holding all 6 animations
	std::vector<AnimationData> m_animationData;

private:

	void InterpolateData(float mTime);

	void CalculateTranslation(float mTime);
	void CalculateRotation(float mTime);
	XMVECTOR CalculateQuaternion(float x, float y, float z);

	XMFLOAT4 m_v4CurrentPos;
	XMFLOAT4 m_v4CurrentRotation;

	//Name of node attached to this animation data
	std::string m_nodeName;

public:
	void SetNodeName(std::string nodeName) { m_nodeName = nodeName; };
	std::string GetNodeName() { return m_nodeName; };

	XMFLOAT4 GetCurrentPosition() { return m_v4CurrentPos; };
	XMFLOAT4 GetCurrentRotation() { return m_v4CurrentRotation; };
};

