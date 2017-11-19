#include "AnimationComponent.h"



AnimationComponent::AnimationComponent()
{
}

AnimationData * AnimationComponent::GetDataByType(AnimationData::AnimationType animationType)
{
	for (int i = 0; i < m_animationData.size(); i++)
	{
		if (m_animationData[i].GetAnimationType() == animationType)
		{
			return &m_animationData[i];
		}
	}

	return nullptr;

}
