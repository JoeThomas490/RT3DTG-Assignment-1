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

	void UpdateHierarchy(bool mDebug);

	void DrawHierarchy();

	void SetActiveAnimation(int index);

	void AddAnimation(const Animation &mAnim) { m_mAnimations.push_back(mAnim); };
	Animation* GetAnimation(int index) { return &(m_mAnimations[index]); };

	Animation* GetActiveAnimation() { return m_pActiveAnimation; };
	void SetBlendingAnimation(Animation* pBlend);
private:
	std::vector<char*> m_vHierarchyOrder;

	std::map<char*, HierarchialComponent*> m_mHierarchyComponents;
	std::vector<Animation> m_mAnimations;

	void CalculateLocalMatrices();
	void CalculateWorldMatrices();
	void UpdateAnimations(bool mDebug);

	XMVECTOR CalculateQuaternion(XMFLOAT4 mRot);

	XMFLOAT4 m_v4LocalPos;
	XMFLOAT4 m_v4LocalRot;

	XMMATRIX m_mLocalMatrix;

	XMFLOAT4 m_v4LocalScale;

	Animation* m_pActiveAnimation;
	Animation* m_pBlendingAnimation;

	float m_fBlendTimer;

	int m_iFrameCounter;


public:

	void SetLocalPosition(XMFLOAT4 mPos) { m_v4LocalPos = mPos; };
	void SetLocalPosition(float fX, float fY, float fZ) { m_v4LocalPos = XMFLOAT4(fX, fY, fZ, 0); };
	void SetLocalRotation(XMFLOAT4 mRot) { m_v4LocalRot = mRot; };
	void SetLocalRotation(float fX, float fY, float fZ) { m_v4LocalRot = XMFLOAT4(fX, fY, fZ, 0); };

};

