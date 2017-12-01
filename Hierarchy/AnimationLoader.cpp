#include "AnimationLoader.h"

//*********************************************************************************************
//************                           Constructor/Destructor                ****************
//*********************************************************************************************

AnimationLoader::AnimationLoader()
{
}

AnimationLoader::~AnimationLoader()
{
}

//*********************************************************************************************
//************                           Public Load Function                  ****************
//*********************************************************************************************

Animation AnimationLoader::LoadXML(const char * fileName)
{
	TiXmlDocument mCurrentXmlDoc = LoadXMLDocument(fileName);

	//Get first node "library_animations"
	TiXmlNode* headNode = mCurrentXmlDoc.FirstChild();

	//Create a new animation
	Animation animation;

	//Create a new animation component
	AnimationComponent component;

	//Set the node name of this component to empty
	component.SetNodeName("");

	//Loop through each animation node 
	for (TiXmlNode* animationNode = headNode->FirstChild("animation"); animationNode != NULL; animationNode = animationNode->NextSibling())
	{
		//Get the component name out of the document
		string componentName = GetAnimationComponentName(animationNode->ToElement());

		//If the components's name is empty (i.e the first component)
		if (*component.GetNodeName() == "")
		{
			//Set the name of our local component
			component.SetNodeName(componentName);
		}
		//If the component's name is different to the one being read (i.e a new component)
		else if (componentName != *component.GetNodeName())
		{
			//Push this component into our animation's component array
			animation.m_vAnimationComponents.push_back(component);
			//Clear the animation data from our current component
			component.m_animationData.clear();
			//Set the name of our component to the one being read
			component.SetNodeName(componentName);
		}

		//Parse the animation tag and populate our component with the correct data
		ParseAnimationTag(animationNode, &component);
	}

	//Push the last component into our animation's component array
	animation.m_vAnimationComponents.push_back(component);

	return animation;
}

//*********************************************************************************************
//************                           Load Document                         ****************
//*********************************************************************************************

TiXmlDocument AnimationLoader::LoadXMLDocument(const char * fileName)
{
	TiXmlDocument doc = TiXmlDocument(fileName);
	doc.LoadFile(fileName);
	return doc;
}

//*********************************************************************************************
//************                           Parse XML                             ****************
//*********************************************************************************************

void AnimationLoader::ParseAnimationTag(Node* animationNode, AnimationComponent* animComponent)
{
	AnimationData data;

	//Get the animation type e.g component-"root" & TRANSLATE_X
	//and populate our animation data with it
	GetAnimationType(animationNode->ToElement(), data);

	//If our data's animation type is valid then continue
	if (data.GetAnimationType() != AnimationData::OTHER)
	{
		//Get source node that has float array (input-array)
		Node* sourceNode = animationNode->FirstChild();

		//Parse this float array collecting the time values
		std::vector<double> timeValues = ParseFloatArray(sourceNode, data);

		//Get the sibiling float array node (output-array)
		sourceNode = sourceNode->NextSibling();

		//Parse this float array collecting the position values
		std::vector<double> positionValues = ParseFloatArray(sourceNode, data);

		//If we're not reading a translate animation
		if (data.GetAnimationType() != AnimationData::TRANSLATE_X)
		{
			//ADD ROTATION DATA

			//Loop through our values and add it into the data
			for (int i = 0; i < timeValues.size(); i++)
			{
				data.AddAnimationElement(timeValues[i], positionValues[i]);
			}

			//If there is more than one value
			if (timeValues.size() > 1)
			{
				//Add the first value of the float array back onto the end with some time added on
				//This creates a smooth transition for looping animations from end to start
				data.AddAnimationElement(timeValues[timeValues.size() - 1] + 0.1f, positionValues[0]);
			}

			//Add the completed animation data object to our animation component
			animComponent->m_animationData.push_back(data);
		}
		else
		{
			//ADD TRANSLATION DATA

			//As it's a translation float-array all x,y and z components are all in one 
			//so we create all three objects at once
			AnimationData xTranslateData, yTranslateData, zTranslateData;

			//Set the correct animation types
			xTranslateData.SetAnimationType(AnimationData::TRANSLATE_X);
			yTranslateData.SetAnimationType(AnimationData::TRANSLATE_Y);
			zTranslateData.SetAnimationType(AnimationData::TRANSLATE_Z);

			for (int i = 0; i < timeValues.size(); i++)
			{
				//Add the data from the float array into the correct data. The order in the collada file
				//goes xVal, yVal, zVal, xVal, yVal ...
				//Each value is divided by 10 as well due to the scaling difference between Maya and DX
				xTranslateData.AddAnimationElement(timeValues[i], positionValues[0 + (i * 3)] / 10.0f);
				yTranslateData.AddAnimationElement(timeValues[i], positionValues[1 + (i * 3)] / 10.0f);
				zTranslateData.AddAnimationElement(timeValues[i], positionValues[2 + (i * 3)] / 10.0f);
			}


			if (timeValues.size() > 1)
			{
				//Add the first value of the float array back onto the end with some time added on
				//This creates a smooth transition for looping animations from end to start
				xTranslateData.AddAnimationElement(timeValues[timeValues.size() - 1] + 0.1f, positionValues[0] / 10.0f);
				yTranslateData.AddAnimationElement(timeValues[timeValues.size() - 1] + 0.1f, positionValues[1] / 10.0f);
				zTranslateData.AddAnimationElement(timeValues[timeValues.size() - 1] + 0.1f, positionValues[2] / 10.0f);
			}
			

			//Add the completed animation data objects to our animation component
			animComponent->m_animationData.push_back(xTranslateData);
			animComponent->m_animationData.push_back(yTranslateData);
			animComponent->m_animationData.push_back(zTranslateData);
		}
	}
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
	std::vector<string> splitVal = Utils::StringUtils::Split(arrayElement->GetText(), ' ');
	std::vector<double> values = Utils::StringUtils::StringToDouble(splitVal);

	//Get accessor node
	Node* accessorNode = arrayNode->NextSibling()->FirstChild();
	Element* accessorElement = accessorNode->ToElement();

	//Get accessor source, count and stride (COULD NEED THIS?) 
	string accessorSource = accessorElement->Attribute("source");
	string accessorCount = accessorElement->Attribute("count");
	string accessorStride = accessorElement->Attribute("stride");

	return values;
}

//*********************************************************************************************
//************                           Get Specific Data                     ****************
//*********************************************************************************************

void AnimationLoader::GetAnimationType(Element * animationElement, AnimationData& data)
{
	//Get id and value from animation tag e.g "root.translate"
	Attribute* animationTypeAttribute = animationElement->FirstAttribute();

	//Get the string value of this attribute
	string animType = animationTypeAttribute->Value();

	//Split this string using a '.'
	std::vector<string> animTypeVec = Utils::StringUtils::Split(animType, '.');

	//Get the type of animation (translate etc.) and set our data's type to it
	data.SetAnimationType(AnimationTypeFromString(animTypeVec[1]));
}

string AnimationLoader::GetAnimationComponentName(Element * animationElement)
{
	Attribute* animationTypeAttribute = animationElement->FirstAttribute();
	string animType = animationTypeAttribute->Value();
	std::vector<string> animTypeVec = Utils::StringUtils::Split(animType, '.');

	return animTypeVec[0];
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


