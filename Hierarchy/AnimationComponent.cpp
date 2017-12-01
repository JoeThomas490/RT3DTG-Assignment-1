#include "AnimationComponent.h"

//*********************************************************************************************
//************                           Constructor/Destructor                ****************
//*********************************************************************************************

AnimationComponent::AnimationComponent()
{
	//Initialise position and rotation to zero
	m_v4CurrentPos = XMFLOAT4(0, 0, 0, 0);
	m_v4CurrentRotation = XMFLOAT4(0, 0, 0, 0);
}

AnimationComponent::~AnimationComponent()
{
}

//*********************************************************************************************
//************                           Update				                   ****************
//*********************************************************************************************

void AnimationComponent::Update(float mTime)
{
	//Lerp and calculate the translation components
	InterpolateTranslationData(mTime);
	//Slerp and calculate the rotation components
	InterpolateRotationData(mTime);
}

//*********************************************************************************************
//************                           Interpolate Functions                 ****************
//*********************************************************************************************

void AnimationComponent::InterpolateTranslationData(float mTime)
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

void AnimationComponent::InterpolateRotationData(float mTime)
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
	XMVECTOR quart1 = Utils::Quaternion::CalculateQuaternion(eulerX1, eulerY1, eulerZ1);
	XMVECTOR quart2 = Utils::Quaternion::CalculateQuaternion(eulerX2, eulerY2, eulerZ2);

	//Slerp between each quaternion and get T for lerp from data using current time
	XMVECTOR quart = XMQuaternionSlerp(quart1, quart2, data->GetT(mTime));

	//Store end result into current rotation
	XMStoreFloat4(&m_v4CurrentRotation, quart);
}



