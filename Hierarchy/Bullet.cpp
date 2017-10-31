#include "Bullet.h"

//Initialise static class variables

CommonMesh* Bullet::s_pBulletMesh = nullptr;

bool Bullet::s_bResourcesReady = false;

Bullet::Bullet(XMFLOAT4 mPos, XMFLOAT4 mRot, float fSpeed)
{

	//Initialise world matrix
	m_mWorldMatrix = XMMatrixIdentity();

	//Initialise start position
	m_v4Pos = mPos;
	m_v4Rot = mRot;

	//TODO: Pass forward vector through at construction
	m_vForwardVector = XMVectorZero();

	//Set movement speed
	m_fMovementSpeed = fSpeed;
}


Bullet::~Bullet()
{
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
	UpdateMatrices();
}

void Bullet::Draw()
{
	Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
	s_pBulletMesh->Draw();
}

void Bullet::UpdateMatrices()
{
	XMMATRIX mTrans, mScale, mRotX, mRotY, mRotZ;
	
	mRotX = XMMatrixRotationX(m_v4Rot.x);
	mRotY = XMMatrixRotationX(m_v4Rot.y);
	mRotZ = XMMatrixRotationX(m_v4Rot.z);

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4Pos));
	mScale = XMMatrixScaling(0.1f, 0.1f, 0.1f);

	m_mWorldMatrix = mRotX * mRotY * mRotZ * mScale * mTrans;
}
