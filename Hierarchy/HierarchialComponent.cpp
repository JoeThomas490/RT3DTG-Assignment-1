#include "HierarchialComponent.h"



HierarchialComponent::HierarchialComponent()
{
	m_mLocalMatrix = XMMatrixIdentity();
	m_mWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0, 0, 0, 0);
	m_v4Pos = XMFLOAT4(0, 0, 0, 0);
}

XMMATRIX HierarchialComponent::UpdateLocalMatrix()
{
	XMMATRIX mRotX, mRotY, mRotZ, mTrans;

	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4Rot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4Rot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4Rot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4Pos));

	m_mLocalMatrix = mRotZ * mRotX * mRotY * mTrans;

	return m_mLocalMatrix;
}

void HierarchialComponent::SetWorldMatrix(XMMATRIX* mWorldMatrix)
{
	m_mWorldMatrix = *mWorldMatrix;
}

void HierarchialComponent::SetLocalRotation(float fX, float fY, float fZ)
{
	m_v4Rot = XMFLOAT4(fX, fY, fZ, 0.0f);
}

void HierarchialComponent::SetLocalPosition(float fX, float fY, float fZ)
{
	m_v4Pos = XMFLOAT4(fX, fY, fZ, 0.0f);
}

void HierarchialComponent::Draw(CommonMesh * mesh)
{
	Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
	mesh->Draw();
}

