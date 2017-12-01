#include "Utils.h"

namespace Utils
{
	namespace Quaternion
	{
		XMVECTOR CalculateQuaternion(float fX, float fY, float fZ)
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
		XMVECTOR CalculateQuaternion(XMFLOAT4 euler)
		{
			//Convert stored angle into radians
			double radX = XMConvertToRadians(euler.x);
			double radY = XMConvertToRadians(euler.y);
			double radZ = XMConvertToRadians(euler.z);

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
	}


	namespace MathsUtils
	{
		float Lerp(float a, float b, float t)
		{
			return ((1 - t) * a) + (t * b);
		}
	}

	namespace StringUtils
	{
		std::vector<std::string> Split(const std::string &txt, char ch)
		{
			//Find position in string that contains specified character
			unsigned int pos = txt.find(ch);
			unsigned int startPos = 0;

			std::vector<std::string> vals;
			std::string t;

			//Loop while our pos value is valid
			while (pos != std::string::npos) {

				//Get the substring from the initial position to found pos
				t = txt.substr(startPos, pos - startPos);
				//Add string
				vals.push_back(t);

				//Update our start position
				startPos = pos + 1;

				//Try and find the specified character again. If it can't be found
				//find() will return string::npos
				pos = txt.find(ch, startPos);
			}

			// Add the last string
			vals.push_back(txt.substr(startPos, txt.size() - startPos));

			return vals;
		}

		std::vector<double> StringToDouble(const std::vector<std::string>& txt)
		{
			std::vector<double> temp;

			for (auto& str : txt)
			{
				temp.push_back(std::stod(str));
			}

			return temp;
		}

	}
}