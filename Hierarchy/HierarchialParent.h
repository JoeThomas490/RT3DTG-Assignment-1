#pragma once

#include <vector>
#include <map>	


#include "Application.h"
#include "HierarchialComponent.h"

#include "Animation.h"

class HierarchialParent
{
public:
	HierarchialParent();
	HierarchialParent(XMFLOAT4 startPos, XMFLOAT4 startRot);
	
	~HierarchialParent() = default;

	void AddHierarchyComponent(HierarchialComponent* mComponent, char* tag);

	HierarchialComponent* GetHiararchyComponentFromTag(char* tag);

	void UpdateHierarchy();

	void DrawHierarchy();

	void SetAnimation(Animation mAnim) { m_animation = mAnim; };

	void SetOffsetForComponent(char* tag, XMFLOAT4 m_offset);


private:
	std::vector<char*> m_vHierarchyOrder;

	std::map<char*, HierarchialComponent*> m_mHierarchyComponents;

	void CalculateLocalMatrices();
	void CalculateWorldMatrices();

	XMFLOAT4 m_v4LocalPos;
	XMFLOAT4 m_v4LocalRot;

	XMFLOAT4 m_v4LocalScale;

	Animation m_animation;
};

