#include "HierarchialComponent.h"



HierarchialComponent::HierarchialComponent()
{
	m_mLocalMatrix = XMMatrixIdentity();
	m_mWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0, 0, 0, 0);
	m_v4RotQuart = XMFLOAT4(0, 0, 0, 0);
	m_v4Pos = XMFLOAT4(0, 0, 0, 0);

	m_bIsDrawable = false;

	m_cParentNode = "";
}

HierarchialComponent::HierarchialComponent(const std::string& parentNode)
{
	m_mLocalMatrix = XMMatrixIdentity();
	m_mWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0, 0, 0, 0);
	m_v4RotQuart = XMFLOAT4(0, 0, 0, 0);
	m_v4Pos = XMFLOAT4(0, 0, 0, 0);

	m_bIsDrawable = false;

	m_cParentNode = parentNode;
}

HierarchialComponent::HierarchialComponent(const std::string & parentNode, CommonMesh * mesh)
{
	m_mLocalMatrix = XMMatrixIdentity();
	m_mWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0, 0, 0, 0);
	m_v4RotQuart = XMFLOAT4(0, 0, 0, 0);
	m_v4Pos = XMFLOAT4(0, 0, 0, 0);

	m_cParentNode = parentNode.c_str();

	m_mMesh = mesh;

	m_bIsDrawable = true;
}

XMMATRIX HierarchialComponent::UpdateLocalMatrix()
{
	
	XMVECTOR mQuart1 = XMLoadFloat4(&m_v4RotQuart);
	
	XMVECTOR mQuart2 = Utils::Quaternion::CalculateQuaternion(m_v4Rot.x, m_v4Rot.y, m_v4Rot.z);

	XMMATRIX mRot = XMMatrixRotationQuaternion(mQuart1) * XMMatrixRotationQuaternion(mQuart2);

	XMMATRIX mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4Pos));

	//XMMATRIX mScale = XMMatrixScalingFromVector(XMLoadFloat4())

	m_mLocalMatrix = mRot * mTrans;

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

void HierarchialComponent::SetColor(float r, float g, float b, float a)
{
	for (int i = 0; i < m_mMesh->GetNumSubsets(); i++)
	{
		m_mMesh->SetColour(i, XMFLOAT4(r,g,b,a));
	}
}

void HierarchialComponent::SetShader(Application::Shader * pShader)
{
	m_mMesh->SetShaderForAllSubsets(pShader);
}

void HierarchialComponent::Draw()
{
	Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
	if (m_bIsDrawable)
		m_mMesh->Draw();
}

