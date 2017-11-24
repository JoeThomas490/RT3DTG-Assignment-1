#include "Bullet.h"

//Initialise static class variables

CommonMesh* Bullet::s_pBulletMesh = nullptr;

bool Bullet::s_bResourcesReady = false;

Bullet::Bullet()
{
	m_mWorldMatrix = XMMatrixIdentity();

	m_v4Pos = XMFLOAT4(0, 0, 0, 0);
	m_v4Rot = XMFLOAT4(0, 0, 0, 0);
	m_v4ForwardVector = XMFLOAT4(0, 0, 0, 0);

	m_vBulletOffset = XMVectorSet(0.0f, 0.2f, 1.5f, 1.0f);

	m_fMovementSpeed = 0;

	m_bIsReadyToUpdate = false;
}

void Bullet::ResetBullet(const XMMATRIX& mGunWorldMatrix, XMFLOAT4 mPlaneForwardVector, float fSpeed)
{

	//Set the initial offset
	XMVECTOR bulletWorldPos = XMVector3Transform(m_vBulletOffset, mGunWorldMatrix);

	XMVECTOR mGunForwardVector = XMVector4Transform(XMVectorSet(0, 0, 1, 0), mGunWorldMatrix);

	//Store positions and rotations from matrix
	XMStoreFloat4(&m_v4Pos, bulletWorldPos);

	//Initialise world matrix
	m_mWorldMatrix = XMMatrixIdentity();

	XMVECTOR rot, scale, trans;
	XMMatrixDecompose(&scale, &rot, &trans, mGunWorldMatrix);

	XMStoreFloat4(&m_v4Rot, rot);

	//Set bullet's forward vector
	mGunForwardVector = XMVector3Normalize(mGunForwardVector);
	mGunForwardVector += (XMLoadFloat4(&mPlaneForwardVector));
	mGunForwardVector = XMVector3Normalize(mGunForwardVector);

	XMStoreFloat4(&m_v4ForwardVector, mGunForwardVector);

	//Set movement speed
	m_fMovementSpeed = fSpeed;

	//Set the bullet to start off invisible
	m_bIsVisible = true;

	m_bIsReadyToUpdate = false;
}

void Bullet::LoadResources()
{
	s_pBulletMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Bullet/bullet.x");
}

void Bullet::ReleaseResources()
{
	if (s_pBulletMesh != nullptr)
	{
		delete s_pBulletMesh;
		s_pBulletMesh = nullptr;
	}
}

void Bullet::Update()
{
	if (m_bIsVisible)
	{
		m_fLifeTime++;
		if (m_fLifeTime > 300)
		{
			m_fLifeTime = 0;
			m_bIsVisible = false;
		}

		if (m_bIsReadyToUpdate)
		{
			m_v4Pos.x += (m_v4ForwardVector.x * 1.6f);
			m_v4Pos.y += (m_v4ForwardVector.y * 1.6f);
			m_v4Pos.z += (m_v4ForwardVector.z * 1.6f);
		}									  
		else
		{
			m_bIsReadyToUpdate = true;
		}

		UpdateMatrices();
	}
}

void Bullet::Draw()
{
	if (m_bIsVisible)
	{
		Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
		s_pBulletMesh->Draw();
	}
}

void Bullet::UpdateMatrices()
{
	XMMATRIX mTrans, mScale, mRot;

	mRot = XMMatrixRotationQuaternion(XMLoadFloat4(&m_v4Rot));
	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4Pos));
	mScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);

	m_mWorldMatrix = mRot * mScale * mTrans;
}
