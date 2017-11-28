#pragma once

#include <string>
#include <vector>

#include "tinyxml\tinyxml.h"
#include "Animation.h"
#include "Application.h"

//*********************************************************************************************
// File:			AnimationLoader.h
// Description:		Class to load animation data from collada files using TinyXML.
//					
//					
// Notes:			
// Todo:			-Add an animation manager so the same animation isn't loaded more 
//					 than once
//*********************************************************************************************

//Typedef commonly used classes
typedef std::string string;
typedef TiXmlNode Node;
typedef TiXmlElement Element;
typedef TiXmlAttribute Attribute;

class AnimationLoader
{
public:

	AnimationLoader();
	~AnimationLoader() = default;

	//Main function to be called when loading an xml file
	Animation LoadXML(const char* fileName);
private:

	TiXmlDocument LoadXMLDocument(const char* fileName);

	//Parse a single animation tag
	void ParseAnimationTag(Node* node, AnimationComponent* componentData);

	//Get the animation type from a element
	void GetAnimationType(Element* animationElement, AnimationData& data);

	//Get the components name from animation tag
	string GetAnimationComponentName(Element * animationElement);

	//Parse a float array and collect values in a vector
	std::vector<double> ParseFloatArray(Node* node, AnimationData& data);
	
	//Split a string into an array by a specific character
	std::vector<string> Split(const string &txt, char ch);

	//Convert a vector of strings into a vector of doubles
	std::vector<double> StringToDouble(const std::vector<string> &txt);

	//Get the enum type AnimationType from a string
	AnimationData::AnimationType AnimationTypeFromString(const string &txt);
};

