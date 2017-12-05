#include "Bullet.h"

//Initialise static class variables
CommonMesh* Bullet::s_pBulletMesh = nullptr;

//*********************************************************************************************
//************                           Constructor/Destructor                ****************
//*********************************************************************************************

Bullet::Bullet()
{
	m_mWorldMatrix = XMMatrixIdentity();

	m_v4Pos = XMFLOAT4(0, 0, 0, 0);
	m_v4Rot = XMFLOAT4(0, 0, 0, 0);
	m_v4ForwardVector = XMFLOAT4(0, 0, 0, 0);

	m_vBulletOffset = XMVectorSet(0.0f, 0.2f, 0.8f, 0.0f);

	m_fMovementSpeed = 0;
}

Bullet::~Bullet()
{
}

//*********************************************************************************************
//************                           Handling Resources                    ****************
//*********************************************************************************************

void Bullet::LoadResources()
{
	//Load bullet mesh from X file
	s_pBulletMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Bullet/bullet.x");
}

void Bullet::ReleaseResources()
{
	//If our bullet mesh pointer isn't null, delete it 
	if (s_pBulletMesh != nullptr)
	{
		delete s_pBulletMesh;
		s_pBulletMesh = nullptr;
	}
}

//*********************************************************************************************
//************                           Spawn Bullet			               ****************
//*********************************************************************************************

//Reset the bullet at the gun's position
void Bullet::ResetBullet(const XMMATRIX& mGunWorldMatrix, XMFLOAT4 mPlaneForwardVector)
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

	//Set the bullet to start off invisible
	m_bIsVisible = true;
}

//*********************************************************************************************
//************                           Update Functions                      ****************
//*********************************************************************************************

//Main update function
void Bullet::Update()
{
	//Only update the bullet if it's visible
	if (m_bIsVisible)
	{
		//Increment the lifetime of bullet
		m_fLifeTime++;

		//If our bullet has been alive for more than 300 frames, then make it invisible
		if (m_fLifeTime > 300)
		{
			m_fLifeTime = 0;
			m_bIsVisible = false;
		}

		//Update our position by adding on our forward vector
		m_v4Pos.x += (m_v4ForwardVector.x * 1.2f);
		m_v4Pos.y += (m_v4ForwardVector.y * 1.2f);
		m_v4Pos.z += (m_v4ForwardVector.z * 1.2f);

		//Update the world matrices for the 
		UpdateMatrices();
	}
}

//Update the world matrices of the bullet
void Bullet::UpdateMatrices()
{
	XMMATRIX mTrans, mScale, mRot;

	mRot = XMMatrixRotationQuaternion(XMLoadFloat4(&m_v4Rot));
	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4Pos));
	mScale = XMMatrixScaling(0.1f, 0.13f, 0.13f);

	m_mWorldMatrix = mRot * mScale * mTrans;
}

//*********************************************************************************************
//************                           Draw				                   ****************
//*********************************************************************************************

//Draw the bullet
void Bullet::Draw()
{
	if (m_bIsVisible)
	{
		Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
		s_pBulletMesh->Draw();
	}
}


