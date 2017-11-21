#include "AnimationData.h"



AnimationData::AnimationData()
{
	fromInd = 0;
	toInd = 1;
}


AnimationData::~AnimationData()
{
}

void AnimationData::AddAnimationElement(float mTime, float mValue)
{
	m_vTimes.push_back(mTime);
	m_vValues.push_back(mValue);

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
	GetIndex(mTime);

	float t = GetT(mTime);

	if (m_iCount == 1)
	{
		return m_vValues[0];
		//return Lerp(GetT(mTime), 0, m_vValues[0]);
	}

	float value = Lerp(t, m_vValues[fromInd], m_vValues[toInd]);
	return value;
}

int AnimationData::GetIndex(float mTime)
{
	int index = 0;

	for (auto& time : m_vTimes)
	{
		if (mTime > time)
		{
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

void AnimationData::AddOffset(int index, float val)
{
	m_vValues[index] += val;
}

float AnimationData::Lerp(float t, float a, float b)
{
	return ((1 - t) * a) + (t * b);
}
