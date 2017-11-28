#pragma once

#include <vector>


//*********************************************************************************************
// File:			AnimationData.h
// Description:		Class that holds two vectors of data, one for the keyframe times and the other
//					for the value at that keyframe
//					
// Notes:			
// Todo:			-Remove vectors?
//*********************************************************************************************

class AnimationData
{
public:

	AnimationData();
	~AnimationData() = default;

	//Add an element to each vecto
	void AddAnimationElement(float mTime, float mValue);

	//Get a value at a specific index
	float GetValue(int index);

	//Get a value and the corresponding time at a specific index
	void GetTimeValuePair(int index, float& time, float& value);

	//Different animation types
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

	//Get amount of values in animation
	int GetAnimationCount() { return m_iCount; };

	//Interpolate value at a certain time
	float Interpolate(float mTime);
	
	//Get from and to index's for lerp
	int GetIndex(float mTime);

	//Get T for lerp at given time
	float GetT(float mTime);

private:

	float Lerp(float t, float a, float b);


	AnimationType m_animationType;

	//Vector of keyframe times
	std::vector<float> m_vTimes;

	//Vector of values
	std::vector<float> m_vValues;

	int m_iCount;

	int fromInd, toInd;

};

