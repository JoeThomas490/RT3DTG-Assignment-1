#include "AnimationData.h"



AnimationData::AnimationData()
{
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

void AnimationData::GetTimeValuePair(int index, float & time, float & value)
{
	time = m_vTimes[index];
	value = m_vValues[index];
}
