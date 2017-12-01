#pragma once

#include "Application.h"

namespace Utils
{
	namespace Quaternion
	{
		XMVECTOR CalculateQuaternion(float fX, float fY, float fZ);
		XMVECTOR CalculateQuaternion(XMFLOAT4 euler);

	}

	namespace MathsUtils
	{
		float Lerp(float a, float b, float t);
	}

	namespace StringUtils
	{
		//Convert a vector of strings into a vector of doubles
		std::vector<double> StringToDouble(const std::vector<std::string>& txt);

		//Split a string into an array by a specific character
		std::vector<std::string> Split(const std::string &txt, char ch);
	}
}

