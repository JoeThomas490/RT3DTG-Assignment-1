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

	m_fMovementSpeed = 0;

	m_bIsReadyToUpdate = false;
}

Bullet::Bullet(XMFLOAT4 mPos, XMFLOAT4 mRot, XMVECTOR mGunForwardVector, XMFLOAT4 mPlaneForwardVector, float fSpeed)
{

	//Initialise world matrix
	m_mWorldMatrix = XMMatrixIdentity();
	m_v4Pos = mPos;
	m_v4Rot = mRot;

	//Set bullet's forward vector
	mGunForwardVector = XMVector3Normalize(mGunForwardVector);
	mGunForwardVector += XMLoadFloat4(&mPlaneForwardVector);
	mGunForwardVector = XMVector3Normalize(mGunForwardVector);

	XMStoreFloat4(&m_v4ForwardVector, mGunForwardVector);

	//Set movement speed
	m_fMovementSpeed = fSpeed;

	//Set the bullet to start off invisible
	m_bIsVisible = true;

	m_bIsReadyToUpdate = false;

}

void Bullet::ResetBullet(XMFLOAT4 mPos, XMFLOAT4 mRot, XMVECTOR mGunForwardVector, XMFLOAT4 mPlaneForwardVector, float fSpeed)
{
	//Initialise world matrix
	m_mWorldMatrix = XMMatrixIdentity();
	m_v4Pos = mPos;
	m_v4Rot = mRot;

	//m_mRotation = mRot;

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
		if (m_fLifeTime > 150)
		{
			m_fLifeTime = 0;
			m_bIsVisible = false;
		}

		if (m_bIsReadyToUpdate)
		{
			m_v4Pos.x += m_v4ForwardVector.x;
			m_v4Pos.y += m_v4ForwardVector.y;
			m_v4Pos.z += m_v4ForwardVector.z;
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
	XMMATRIX mTrans, mScale, mRotX, mRotY, mRotZ;

	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4Rot.x));
	mRotY = XMMatrixRotationX(XMConvertToRadians(m_v4Rot.y));
	mRotZ = XMMatrixRotationX(XMConvertToRadians(m_v4Rot.z));

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4Pos));
	mScale = XMMatrixScaling(0.1f, 0.1f, 0.1f);


	m_mWorldMatrix = mRotX * mRotY * mRotZ * mScale * mTrans;

}
