#pragma once

#include <string>
#include <vector>

#include "tinyxml\tinyxml.h"
#include "Animation.h"
#include "Application.h"

//*********************************************************************************************
// File:			AnimationLoader.h
// Description:		 
//					
//					
// Notes:			
// Todo:			
//*********************************************************************************************

typedef std::string string;
typedef TiXmlNode Node;
typedef TiXmlElement Element;
typedef TiXmlAttribute Attribute;

class AnimationLoader
{
public:
	AnimationLoader();

	Animation LoadXML(const char* fileName);

	~AnimationLoader() = default;

private:
	TiXmlDocument LoadXMLDocument(const char* fileName);

	void ParseAnimationTag(Node* node, AnimationComponent* componentData);
	void ParseAnimationType(Element* animationElement, AnimationData& data);
	std::vector<double> ParseFloatArray(Node* node, AnimationData& data);

	string GetAnimationComponentName(Element * animationElement);

	std::vector<string> Split(const string &txt, char ch);
	std::vector<double> StringToDouble(const std::vector<string> &txt);

	AnimationData::AnimationType AnimationTypeFromString(const string &txt);



private:



	TiXmlDocument m_currentXMLDoc;

	std::vector<AnimationData> m_vAnimationData;

};

