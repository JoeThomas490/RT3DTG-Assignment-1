#include "AnimationComponent.h"

AnimationComponent::AnimationComponent()
{
	//Initialise position and rotation to zero
	m_v4CurrentPos = XMFLOAT4(0, 0, 0, 0);
	m_v4CurrentRotation = XMFLOAT4(0, 0, 0, 0);
}

AnimationData * AnimationComponent::GetDataByType(AnimationData::AnimationType animationType)
{
	//Loop through each element in animation data array
	for (int i = 0; i < m_animationData.size(); i++)
	{
		//If the animation type matches the one passed in 
		if (m_animationData[i].GetAnimationType() == animationType)
		{
			//Then return the correct animation data
			return &m_animationData[i];
		}
	}
	
	//If it can't be found then return a nullptr
	return nullptr;
}

void AnimationComponent::Update(float mTime)
{
	InterpolateData(mTime);
}

void AnimationComponent::InterpolateData(float mTime)
{
	//Lerp and calculate the translation components
	CalculateTranslation(mTime);
	//Slerp and calculate the rotation components
	CalculateRotation(mTime);
}

void AnimationComponent::CalculateTranslation(float mTime)
{
	//Loop through each element in the data array
	for (auto& data : m_animationData)
	{
		//Get the animation type
		AnimationData::AnimationType animType = data.GetAnimationType();

		
		//If the animation type is a translation
		if (animType == AnimationData::TRANSLATE_X || animType == AnimationData::TRANSLATE_Y || animType == AnimationData::TRANSLATE_Z)
		{
			//Get the interpolated value at the given time
			float value = data.Interpolate(mTime);


			//Based on the animation type, put the interpolated value
			//in the corresponding x,y or z position.
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
}

void AnimationComponent::CalculateRotation(float mTime)
{
	//Get x rotation data out of animation data array
	AnimationData* data = &m_animationData[3];

	//Get index based on current time
	int index = data->GetIndex(mTime);

	//Get x rotation value from data and the one to slerp to
	float eulerX1 = data->GetValue(index);
	float eulerX2 = data->GetValue(index + 1);

	//Get y rotation data out of animation data array
	data = &m_animationData[4];

	//Get index based on current time
	index = data->GetIndex(mTime);

	//Get y rotation value from data and the one to slerp to
	float eulerY1 = data->GetValue(index);
	float eulerY2 = data->GetValue(index + 1);

	//Get z rotation data out of animation data array
	data = &m_animationData[5];

	//Get index based on current time
	index = data->GetIndex(mTime);

	//Get z rotation value from data and the one to slerp to
	float eulerZ1 = data->GetValue(index);
	float eulerZ2 = data->GetValue(index + 1);

	//Get the quaternions from the euler angles
	XMVECTOR quart1 = CalculateQuaternion(eulerX1, eulerY1, eulerZ1);
	XMVECTOR quart2 = CalculateQuaternion(eulerX2, eulerY2, eulerZ2);

	//Slerp between each quaternion and get T for lerp from data using current time
	XMVECTOR quart = XMQuaternionSlerp(quart1, quart2, data->GetT(mTime));

	//Store end result into current rotation
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



