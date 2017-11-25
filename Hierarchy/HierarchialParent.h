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

	XMFLOAT4 GetParentPosition();

	void SetActiveAnimation(int index);
	void AddAnimation(const Animation &mAnim) { m_mAnimations.push_back(mAnim); };

	void SetShaderForAll(Application::Shader* pShader);

	bool LoadShader();
	void DeleteShader();
	void UpdateShader(XMFLOAT3 camPos);

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

	Application::Shader m_shader;

	ID3D11Buffer* m_pMyAppCBuffer; // our custom buffer resource.
	int m_psMyAppCBufferSlot; // custom buffer resource binding in PS, discovered by reflection.
	int m_vsMyAppCBufferSlot; // custom buffer resource binding in VS, discovered by reflection.
	int m_cameraPosOffset;

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

