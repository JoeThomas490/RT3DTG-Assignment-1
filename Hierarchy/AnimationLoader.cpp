#include "AnimationLoader.h"


AnimationLoader::AnimationLoader()
{
}


Animation AnimationLoader::LoadXML(const char * fileName)
{
	m_currentXMLDoc = LoadXMLDocument(fileName);

	//Get first node "library_animations"
	TiXmlNode* headNode = m_currentXMLDoc.FirstChild();

	Animation animation;

	AnimationComponent component;
	component.SetNodeName("");

	//Get first "animation" node
	for (TiXmlNode* animationNode = headNode->FirstChild("animation"); animationNode != NULL; animationNode = animationNode->NextSibling())
	{
		string componentName = GetAnimationComponentName(animationNode->ToElement());
		if (*component.GetNodeName() == "")
		{
			component.SetNodeName(componentName);
		}
		else if (componentName != *component.GetNodeName())
		{
			animation.m_vAnimationComponents.push_back(component);
			component.m_animationData.clear();
			component.SetNodeName(componentName);
		}
		ParseAnimationTag(animationNode, &component);
	}

	animation.m_vAnimationComponents.push_back(component);

	return animation;
}

TiXmlDocument AnimationLoader::LoadXMLDocument(const char * fileName)
{
	TiXmlDocument doc = TiXmlDocument(fileName);
	doc.LoadFile(fileName);
	return doc;
}

void AnimationLoader::ParseAnimationTag(Node* animationNode, AnimationComponent* componentData)
{
	AnimationData data;
	//Get the animation type e.g component-"root" & TRANSLATE_X
	ParseAnimationType(animationNode->ToElement(), data);

	if (data.GetAnimationType() != AnimationData::OTHER)
	{
		//Get source node
		Node* sourceNode = animationNode->FirstChild();

		std::vector<double> timeValues = ParseFloatArray(sourceNode, data);

		sourceNode = sourceNode->NextSibling();

		std::vector<double> positionValues = ParseFloatArray(sourceNode, data);

		//If we're not reading a translate animation
		if (data.GetAnimationType() != AnimationData::TRANSLATE_X)
		{
			for (int i = 0; i < timeValues.size(); i++)
			{
				data.AddAnimationElement(timeValues[i], positionValues[i]);
			}

			if (timeValues.size() > 1)
				data.AddAnimationElement(timeValues[timeValues.size() - 1] + 0.1f, positionValues[0]);

			componentData->m_animationData.push_back(data);
		}
		else
		{
			AnimationData xTranslateData, yTranslateData, zTranslateData;
			xTranslateData.SetAnimationType(AnimationData::TRANSLATE_X);
			yTranslateData.SetAnimationType(AnimationData::TRANSLATE_Y);
			zTranslateData.SetAnimationType(AnimationData::TRANSLATE_Z);

			for (int i = 0; i < timeValues.size(); i++)
			{
				xTranslateData.AddAnimationElement(timeValues[i], positionValues[0 + (i * 3)] / 10.0f);
				yTranslateData.AddAnimationElement(timeValues[i], positionValues[1 + (i * 3)] / 10.0f);
				zTranslateData.AddAnimationElement(timeValues[i], positionValues[2 + (i * 3)] / 10.0f);
			}

			if (timeValues.size() > 1)
			{
				xTranslateData.AddAnimationElement(timeValues[timeValues.size() - 1] + 0.1f, positionValues[0] / 10.0f);
				yTranslateData.AddAnimationElement(timeValues[timeValues.size() - 1] + 0.1f, positionValues[1] / 10.0f);
				zTranslateData.AddAnimationElement(timeValues[timeValues.size() - 1] + 0.1f, positionValues[2] / 10.0f);
			}
			

			componentData->m_animationData.push_back(xTranslateData);
			componentData->m_animationData.push_back(yTranslateData);
			componentData->m_animationData.push_back(zTranslateData);
		}
	}
}

void AnimationLoader::ParseAnimationType(Element * animationElement, AnimationData& data)
{
	//Get id and value from animation tag e.g "root.translate"
	Attribute* animationTypeAttribute = animationElement->FirstAttribute();
	string animType = animationTypeAttribute->Value();
	std::vector<string> animTypeVec = Split(animType, '.');

	//And get the type of animation (translate etc.)
	data.SetAnimationType(AnimationTypeFromString(animTypeVec[1]));
}

string AnimationLoader::GetAnimationComponentName(Element * animationElement)
{
	Attribute* animationTypeAttribute = animationElement->FirstAttribute();
	string animType = animationTypeAttribute->Value();
	std::vector<string> animTypeVec = Split(animType, '.');

	return animTypeVec[0];
}

std::vector<double> AnimationLoader::ParseFloatArray(Node * sourceNode, AnimationData& data)
{
	//Get input array node
	Node* arrayNode = sourceNode->FirstChild();
	Element* arrayElement = arrayNode->ToElement();

	//Get id value (DON'T NEED THIS BUT COULD BE USEFUL)
	string arrayID = arrayElement->Attribute("id");

	//Get count value (COULD NEED THIS?)
	string arrayCount = arrayElement->Attribute("count");

	//Create vector of time values by splitting value inside tag (IMPORTANT BITS)
	std::vector<string> splitVal = Split(arrayElement->GetText(), ' ');
	std::vector<double> values = StringToDouble(splitVal);

	//Get accessor node
	Node* accessorNode = arrayNode->NextSibling()->FirstChild();
	Element* accessorElement = accessorNode->ToElement();

	//Get accessor source, count and stride (COULD NEED THIS?) 
	string accessorSource = accessorElement->Attribute("source");
	string accessorCount = accessorElement->Attribute("count");
	string accessorStride = accessorElement->Attribute("stride");

	return values;
}

std::vector<string> AnimationLoader::Split(const string &txt, char ch)
{
	unsigned int pos = txt.find(ch);
	unsigned int initialPos = 0;

	std::vector<string> vals;
	string t;

	// Decompose statement
	while (pos != std::string::npos) {
		t = txt.substr(initialPos, pos - initialPos);
		vals.push_back(t);

		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	vals.push_back(txt.substr(initialPos, txt.size() - initialPos));

	return vals;
}

std::vector<double> AnimationLoader::StringToDouble(const std::vector<string>& txt)
{
	std::vector<double> temp;

	for (auto& str : txt)
	{
		temp.push_back(std::stod(str));
	}

	return temp;
}

AnimationData::AnimationType AnimationLoader::AnimationTypeFromString(const string & txt)
{
	if (txt == "translate")
	{
		return AnimationData::TRANSLATE_X;
	}
	else if (txt == "rotateX")
	{
		return AnimationData::ROTATE_X;
	}
	else if (txt == "rotateY")
	{
		return AnimationData::ROTATE_Y;
	}
	else if (txt == "rotateZ")
	{
		return AnimationData::ROTATE_Z;
	}
	else
	{
		return AnimationData::OTHER;
	}
}


