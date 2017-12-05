#pragma once

#include "Application.h"

#include <D3DX11tex.h>


//*********************************************************************************************
// File:			CubeMap.h
// Description:		Class holding logic for a CubeMap skybox. Creates a sphere mesh and applies a 
//					shader to create the effect of a skybox. Also handles matrix and drawing.
// Notes:			
// Todo:			-Create a TextureManager and make this generic so it can take any cubemap texture
//					
//*********************************************************************************************

__declspec(align(16)) class CubeMap
{
public:

	CubeMap();
	~CubeMap();

	//Create depth stencil for our shader
	void CreateDepthStencil();

	//Load texture for cubemap
	void LoadResources();

	//Load shader for and set corresponding buffers
	bool ReloadShader();
	//Delete shader
	void DeleteShader();

	//Update position of skybox using camera position
	void Update(const XMFLOAT3& camPos);
	
	//Draw the cubemap, passing the frame count for shader
	void Draw(float mFrameCount);

private:

	//World matrix for the cubemap
	XMMATRIX m_mWorldMatrix;

	//Pointer to the mesh of the sphere
	CommonMesh* m_mesh;

	//Shader used for cubemap
	Application::Shader m_mShader;

	//Constant buffer "MyApp" in shader
	ID3D11Buffer* m_pMyAppCBuffer; // our custom buffer resource.

	//Offset for PS MyApp Constant buffer
	int m_psMyAppCBufferSlot;

	//Offset for VS MyApp Constant buffer
	int m_vsMyAppCBufferSlot;

	//Offset for frame count in constant buffer
	int m_frameCountOffset;

	//Offset in shader for texture
	int m_psTexture0;
	
	ID3D11ShaderResourceView* m_pShaderResourceView;

	ID3D11SamplerState* m_pSamplerState;
	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

public:

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

