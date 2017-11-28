#include "AnimationData.h"

AnimationData::AnimationData()
{
	fromInd = 0;
	toInd = 1;
}

void AnimationData::AddAnimationElement(float mTime, float mValue)
{
	//Add time to vector
	m_vTimes.push_back(mTime);

	//Add value to vector
	m_vValues.push_back(mValue);

	//Update the data count 
	m_iCount = m_vTimes.size();
}

float AnimationData::GetValue(int index)
{
	return m_vValues[index];
}

void AnimationData::GetTimeValuePair(int index, float & time, float & value)
{
	time = m_vTimes[index];
	value = m_vValues[index];
}

float AnimationData::Interpolate(float mTime)
{
	//Todo Check if there's less than one 

	//If there's only one value in the animation data
	if (m_iCount == 1)
	{
		//Just return the first value
		return m_vValues[0];
	}

	//Get from and to index's
	GetIndex(mTime);

	//Get T for lerp
	float t = GetT(mTime);

	//Lerp the two values from the data and return it
	return Lerp(t, m_vValues[fromInd], m_vValues[toInd]);
}

int AnimationData::GetIndex(float mTime)
{
	int index = 0;

	//Loop through all the times
	for (auto& time : m_vTimes)
	{
		//If our current time is greater than this time
		if (mTime > time)
		{
			//Update the from and to index's for lerping
			fromInd = index;
			toInd = index + 1;
		}

		index++;
	}

	return fromInd;
}

float AnimationData::GetT(float mTime)
{
	if (m_iCount > 1)
	{
		float fromTime = m_vTimes[fromInd];
		float toTime = m_vTimes[toInd];

		float t = (mTime - fromTime) / (toTime - fromTime);
		return t;
	}
	else
	{
		float fromTime = 0;
		float toTime = m_vTimes[0];

		float t = (mTime - fromTime) / toTime - fromTime;
		return t;
	}
	
	return 0;
}

float AnimationData::Lerp(float t, float a, float b)
{
	return ((1 - t) * a) + (t * b);
}
