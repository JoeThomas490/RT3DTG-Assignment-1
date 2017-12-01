#pragma once

#include "Application.h"

#include "D3DX11tex.h"
#include "d3dx11effect.h"

__declspec(align(16)) class CubeMap
{
public:
	CubeMap();

	void LoadResources();

	bool ReloadShader();
	void DeleteShader();

	void Update(const XMFLOAT3& camPos);
	void Draw(float mFrameCount);

	~CubeMap();

	CommonMesh* m_mesh;


private:
	Application::Shader m_mShader;


	ID3D11Buffer* m_pMyAppCBuffer; // our custom buffer resource.
	int m_psMyAppCBufferSlot; // custom buffer resource binding in PS, discovered by reflection.
	int m_vsMyAppCBufferSlot; // custom buffer resource binding in VS, discovered by reflection.

	float m_frameCount;

	int m_frameCountOffset;
	int m_psTexture0;


	XMMATRIX m_mWorldMatrix;

	ID3D11Texture2D* m_pTextures[1];
	ID3D11ShaderResourceView* smrv;
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

