#pragma once

#include <vector>
#include <map>	
#include <fstream>

#include "Application.h"
#include "HierarchialComponent.h"
#include "Animation.h"
#include "MeshManager.h"


//*********************************************************************************************
// File:			HierarchialParent.h
// Description:		Class to handle an animation component tree. Manages update of all components
//					by calculateing local and world matrices with translation and rotation
//					
//					
// Notes:			
// Todo:			-Add the ability to read from a file to create a hierarchy
//					-Create a class to handle shader stuff
//*********************************************************************************************

class HierarchialParent
{
public:

	HierarchialParent();
	HierarchialParent(XMFLOAT4 startPos, XMFLOAT4 startRot);
	
	virtual ~HierarchialParent() = default;

	void LoadHierarchyFromFile(const char* mFileName, const char* mEntity);

	HierarchialComponent* AddHierarchyComponent(HierarchialComponent* mComponent, const string& tag);

	HierarchialComponent* GetHiararchyComponentFromTag(char* tag);

	void UpdateHierarchy(bool mDebug);
	void DrawHierarchy();

	XMFLOAT4 GetParentPosition();

	void SetActiveAnimation(int index);
	void AddAnimation(const Animation &mAnim) { m_mAnimations.push_back(mAnim); };

	void SetShaderForAll(Application::Shader* pShader);

	bool LoadShader();
	void DeleteShader();
	void UpdateShader(XMFLOAT3 camPos, float mFrameCount);


private:
	std::vector<std::string> m_vHierarchyOrder;

	std::map<std::string, HierarchialComponent*> m_mHierarchyComponents;
	std::vector<Animation> m_mAnimations;

	void CalculateLocalMatrices();
	void CalculateWorldMatrices();
	void UpdateAnimations(bool mDebug);

	XMVECTOR CalculateQuaternion(XMFLOAT4 mRot);

	//Split a string into an array by a specific character
	std::vector<std::string> Split(const std::string &txt, char ch);

	//Convert a vector of strings into a vector of doubles
	std::vector<double> StringToDouble(const std::vector<std::string> &txt);

	XMFLOAT4 m_v4LocalPos;
	XMFLOAT4 m_v4LocalRot;

	XMMATRIX m_mLocalMatrix;

	XMFLOAT4 m_v4LocalScale;

	Animation* m_pActiveAnimation;
	Animation* m_pBlendingAnimation;

	float m_fBlendTimer;

	int m_iFrameCounter;

	Application::Shader m_shader;

	ID3D11Buffer* m_pMyAppCBuffer; // our custom buffer resource.
	int m_psMyAppCBufferSlot; // custom buffer resource binding in PS, discovered by reflection.
	int m_vsMyAppCBufferSlot; // custom buffer resource binding in VS, discovered by reflection.
	int m_cameraPosOffset;
	int m_framePosOffset;

public:

	void SetLocalPosition(XMFLOAT4 mPos) { m_v4LocalPos = mPos; };
	void SetLocalPosition(float fX, float fY, float fZ) { m_v4LocalPos = XMFLOAT4(fX, fY, fZ, 0); };
	void SetLocalRotation(XMFLOAT4 mRot) { m_v4LocalRot = mRot; };
	void SetLocalRotation(float fX, float fY, float fZ) { m_v4LocalRot = XMFLOAT4(fX, fY, fZ, 0); };

	Animation* GetAnimation(int index) { return &(m_mAnimations[index]); };
	Animation* GetActiveAnimation() { return m_pActiveAnimation; };
	Animation* GetBlendingAnimation() { return m_pBlendingAnimation; };
	void SetBlendingAnimation(Animation* pBlend);
};

