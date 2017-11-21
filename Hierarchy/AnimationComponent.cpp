#include "AnimationComponent.h"



AnimationComponent::AnimationComponent()
{
	m_v4CurrentPos = XMFLOAT4(0, 0, 0, 0);
	m_v4CurrentRotation = XMFLOAT4(0, 0, 0, 0);
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

void AnimationComponent::Update(float mTime)
{
	InterpolateData(mTime);
}

void AnimationComponent::InterpolateData(float mTime)
{
	CalculateTranslation(mTime);
	CalculateRotation(mTime);
}

void AnimationComponent::CalculateTranslation(float mTime)
{
	for (auto& data : m_animationData)
	{
		AnimationData::AnimationType animType = data.GetAnimationType();

		float value = 0;
		if (animType == AnimationData::TRANSLATE_X || animType == AnimationData::TRANSLATE_Y || animType == AnimationData::TRANSLATE_Z)
		{
			value = data.Interpolate(mTime);
		}

		switch (animType)
		{
		case AnimationData::TRANSLATE_X:
			m_v4CurrentPos.x = value;
			break;
		case AnimationData::TRANSLATE_Y:
			m_v4CurrentPos.y = value;
			break;
		case AnimationData::TRANSLATE_Z:
			m_v4CurrentPos.z = value;
			break;
		}
	}
}

void AnimationComponent::CalculateRotation(float mTime)
{
	float eulerX, eulerY, eulerZ;

	AnimationData* data = GetDataByType(AnimationData::ROTATE_X);
	eulerX = data->GetValue(data->GetIndex(mTime));

	data = GetDataByType(AnimationData::ROTATE_Y);
	eulerY = data->GetValue(data->GetIndex(mTime));

	data = GetDataByType(AnimationData::ROTATE_Z);
	eulerZ = data->GetValue(data->GetIndex(mTime));

	XMVECTOR quart1 = CalculateQuaternion(eulerX, eulerY, eulerZ);

	data = GetDataByType(AnimationData::ROTATE_X);
	eulerX = data->GetValue(data->GetIndex(mTime) + 1);

	data = GetDataByType(AnimationData::ROTATE_Y);
	eulerY = data->GetValue(data->GetIndex(mTime) + 1);

	data = GetDataByType(AnimationData::ROTATE_Z);
	eulerZ = data->GetValue(data->GetIndex(mTime) + 1);

	XMVECTOR quart2 = CalculateQuaternion(eulerX, eulerY, eulerZ);

	XMVECTOR quart = XMQuaternionSlerp(quart1, quart2, data->GetT(mTime));
	XMStoreFloat4(&m_v4CurrentRotation, quart);


}

XMVECTOR AnimationComponent::CalculateQuaternion(float fX, float fY, float fZ)
{
	//Convert stored angle into radians
	double radX = XMConvertToRadians(fX);
	double radY = XMConvertToRadians(fY);
	double radZ = XMConvertToRadians(fZ);

	//Calculate cos components
	double c1 = cos(radY / 2.0);
	double c2 = cos(radZ / 2.0);
	double c3 = cos(radX / 2.0);

	//Calculate sin components
	double s1 = sin(radY / 2.0);
	double s2 = sin(radZ / 2.0);
	double s3 = sin(radX / 2.0);

	//Calculate quaternion value using components
	double w = (c1 * c2 * c3) - (s1 * s2 * s3);
	double x = (s1 * s2 * c3) + (c1 * c2 * s3);
	double y = (s1 * c2 * c3) + (c1 * s2 * s3);
	double z = (c1 * s2 * c3) - (s1 * c2 * s3);

	//Return as an XMVECTOR
	return XMVectorSet(x, y, z, w);
}



