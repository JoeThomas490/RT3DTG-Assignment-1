#include "CubeMap.h"



CubeMap::CubeMap()
{
	m_mesh = CommonMesh::NewSphereMesh(Application::s_pApp, 10, 8, 16);
	m_mWorldMatrix = XMMatrixRotationQuaternion(XMVectorSet(0, 0, 0, 0)) * XMMatrixTranslationFromVector(XMVectorSet(0, 0, 0, 0));

	m_frameCount = 0;

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	Application::s_pApp->GetDevice()->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	m_pMyAppCBuffer = NULL;

	LoadResources();
}



void CubeMap::LoadResources()
{
	ID3D11ShaderResourceView* mCubeMapSRV;
	ID3D11Device* pDevice = Application::s_pApp->GetDevice();

	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* SMTexture = 0;
	D3DX11CreateTextureFromFile(pDevice, L"./Resources/Skybox/skybox.dds", &loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	pDevice->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);

	ReloadShader();
}

bool CubeMap::ReloadShader()
{
	ID3D11VertexShader* pVS = NULL;
	ID3D11PixelShader* pPS = NULL;
	ID3D11InputLayout* pIL = NULL;

	ShaderDescription vs, ps;

	ID3D11Device* pDevice = Application::s_pApp->GetDevice();

	char maxNumLightsValue[100];
	_snprintf_s(maxNumLightsValue, sizeof maxNumLightsValue, _TRUNCATE, "%d", CommonApp::MAX_NUM_LIGHTS);

	D3D_SHADER_MACRO aMacros[] = {
		{
			"MAX_NUM_LIGHTS",
			maxNumLightsValue,
		},
		{ NULL },
	};

	if (!CompileShadersFromFile(pDevice, "./Resources/Shaders/CubeMapShader.hlsl", "VSMain", &pVS, &vs, g_aVertexDesc_Pos3fColour4ubNormal3f,
		g_vertexDescSize_Pos3fColour4ubNormal3f, &pIL, "PSMain", &pPS, &ps, aMacros))
	{
		return false; // false;
	}

	Application::s_pApp->CreateShaderFromCompiledShader(&m_mShader, pVS, &vs, pIL, pPS, &ps);

	ps.FindCBuffer("MyApp", &m_psMyAppCBufferSlot);

	ps.FindTexture("gCubeMap", &m_psTexture0);
	ps.FindFloat(m_psMyAppCBufferSlot, "g_frameCount", &m_frameCountOffset);
	//ps.FindFloat(m_psMyAppCBufferSlot, "g_cameraPosition", &m_cameraPosOffset);
	vs.FindCBuffer("MyApp", &m_vsMyAppCBufferSlot);
	// Create a cbuffer, using the shader description to find out how
	// large it needs to be.
	m_pMyAppCBuffer = CreateBuffer(pDevice, ps.GetCBufferSizeBytes(m_psMyAppCBufferSlot), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL);

	// In this example we are sharing the constant buffer between both vertex and pixel shaders.
	// This is efficient since we only update one buffer. However we could define separate constant buffers for each stage.
	// Generally constant buffers should represent groups of variables that must be updated at the same rate.
	// So : we might have 'per execution' 'per frame', 'per draw' constant buffers.

	m_mesh->SetShaderForAllSubsets(&m_mShader);
}

void CubeMap::DeleteShader()
{
	Release(m_pMyAppCBuffer);

	m_mShader.Reset();
}

void CubeMap::Update(const XMFLOAT3& camPos)
{
	m_mWorldMatrix = XMMatrixIdentity();

	XMMATRIX mScale = XMMatrixScaling(250.0f, 250.0f, 250.0f);
	XMMATRIX mTranslation = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);

	m_mWorldMatrix = mScale * mTranslation;
}

void CubeMap::Draw(float mFrameCount)
{
	ID3D11DeviceContext* pContext = Application::s_pApp->GetDeviceContext();

	if (m_psTexture0 >= 0)
		pContext->PSSetShaderResources(m_psTexture0, 1, &smrv);

	Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);

	if (m_pMyAppCBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		if (SUCCEEDED(pContext->Map(m_pMyAppCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map)))
		{
			// Set the buffer contents. There is only one variable to set in this case.
			// This method relies on the offset which has been found through Shader Reflection.
			SetCBufferFloat(map, m_frameCountOffset, mFrameCount);
			pContext->Unmap(m_pMyAppCBuffer, 0);
		}
	}

	// Bind the same constant buffer to any stages that use it.
	if (m_psMyAppCBufferSlot != -1)
	{
		pContext->PSSetConstantBuffers(m_psMyAppCBufferSlot, 1, &m_pMyAppCBuffer);
	}
	if (m_vsMyAppCBufferSlot != -1)
	{
		pContext->VSSetConstantBuffers(m_vsMyAppCBufferSlot, 1, &m_pMyAppCBuffer);
	}


	m_mesh->Draw();
}

CubeMap::~CubeMap()
{
	DeleteShader();

	smrv->Release();

	DSLessEqual->Release();
	RSCullNone->Release();
}
