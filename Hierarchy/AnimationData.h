#pragma once

#include <vector>


//*********************************************************************************************
// File:			AnimationData.h
// Description:		 
//					
//					
// Notes:			
// Todo:			-Remove vectors?
//*********************************************************************************************

class AnimationData
{
public:
	AnimationData();
	~AnimationData();

	void AddAnimationElement(float mTime, float mValue);

	float GetTime(int index);
	float GetValue(int index);
	void GetTimeValuePair(int index, float& time, float& value);

	enum AnimationType
	{
		TRANSLATE_X,
		TRANSLATE_Y,
		TRANSLATE_Z,
		ROTATE_X,
		ROTATE_Y,
		ROTATE_Z,
		OTHER
	};


public:

	void SetAnimationType(AnimationType animationType) { m_animationType = animationType; };
	AnimationType GetAnimationType() { return m_animationType; };

	int GetAnimationCount() { return m_iCount; };

	float Interpolate(float mTime);
	int GetIndex(float mTime);
	float GetT(float mTime);

	void AddOffset(int index, float val);

private:

	float Lerp(float t, float a, float b);


	AnimationType m_animationType;

	std::vector<float> m_vTimes;
	std::vector<float> m_vValues;

	int m_iCount;

	int fromInd, toInd;

};

