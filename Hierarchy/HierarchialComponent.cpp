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

HierarchialComponent::HierarchialComponent(char * parentNode)
{
	m_mLocalMatrix = XMMatrixIdentity();
	m_mWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0, 0, 0, 0);
	m_v4RotQuart = XMFLOAT4(0, 0, 0, 0);
	m_v4Pos = XMFLOAT4(0, 0, 0, 0);

	m_bIsDrawable = false;

	m_cParentNode = parentNode;
}

HierarchialComponent::HierarchialComponent(char * parentNode, CommonMesh * mesh)
{
	m_mLocalMatrix = XMMatrixIdentity();
	m_mWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0, 0, 0, 0);
	m_v4RotQuart = XMFLOAT4(0, 0, 0, 0);
	m_v4Pos = XMFLOAT4(0, 0, 0, 0);
	m_v4Offset = XMFLOAT4(0, 0, 0, 0);

	m_cParentNode = parentNode;

	m_mMesh = mesh;

	m_bIsDrawable = true;
}

HierarchialComponent::HierarchialComponent(const std::string & parentNode, CommonMesh * mesh)
{
	m_mLocalMatrix = XMMatrixIdentity();
	m_mWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0, 0, 0, 0);
	m_v4RotQuart = XMFLOAT4(0, 0, 0, 0);
	m_v4Pos = XMFLOAT4(0, 0, 0, 0);
	m_v4Offset = XMFLOAT4(0, 0, 0, 0);

	m_cParentNode = parentNode.c_str();

	m_mMesh = mesh;

	m_bIsDrawable = true;
}

XMMATRIX HierarchialComponent::UpdateLocalMatrix()
{
	
	XMVECTOR mQuart1 = XMLoadFloat4(&m_v4RotQuart);
	

	XMVECTOR mQuart2 = CalculateQuaternion(m_v4Rot.x, m_v4Rot.y, m_v4Rot.z);

	XMMATRIX mRot = XMMatrixRotationQuaternion(mQuart1) * XMMatrixRotationQuaternion(mQuart2);

	XMMATRIX mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4Pos));

	//TODO add scaling to each component 
	//XMMATRIX mScale = XMMatrixScalingFromVector(XMLoadFloat4())

	m_mLocalMatrix = mRot * mTrans;

	return m_mLocalMatrix;
}

XMVECTOR HierarchialComponent::CalculateQuaternion(float fX, float fY, float fZ)
{
	//Convert stored angle into radians
	double radX = XMConvertToRadians(fX);
	double radY = XMConvertToRadians(fY);
	double radZ = XMConvertToRadians(fZ);
	
	//Calculate cos components
	double c1 = cos(radY / 2.0);
	double c2 = cos(radZ / 2.0);
	double c3 = cos(radX / 2.0);

	//Calculate sin components
	double s1 = sin(radY / 2.0);
	double s2 = sin(radZ / 2.0);
	double s3 = sin(radX / 2.0);

	//Calculate quaternion value using components
	double w = (c1 * c2 * c3) - (s1 * s2 * s3);
	double x = (s1 * s2 * c3) + (c1 * c2 * s3);
	double y = (s1 * c2 * c3) + (c1 * s2 * s3);
	double z = (c1 * s2 * c3) - (s1 * c2 * s3);

	//Return as an XMVECTOR
	return XMVectorSet(x, y, z, w);
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

void HierarchialComponent::SetLocalOffset(float fX, float fY, float fZ)
{
	m_v4Offset = XMFLOAT4(fX, fY, fZ, 0.0f);
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

