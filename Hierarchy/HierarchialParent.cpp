#include "HierarchialParent.h"



HierarchialParent::HierarchialParent()
{
	m_v4LocalPos = XMFLOAT4(0, 0, 0, 0);
	m_v4LocalRot = XMFLOAT4(0, 0, 0, 0);

	m_pActiveAnimation = nullptr;
	m_pBlendingAnimation = nullptr;

	m_fBlendTimer = 0;
	m_iFrameCounter = 0;

	m_pMyAppCBuffer = NULL;

	LoadShader();
}

HierarchialParent::HierarchialParent(XMFLOAT4 startPos, XMFLOAT4 startRot)
{
	m_v4LocalPos = startPos;
	m_v4LocalRot = startRot;

	m_pActiveAnimation = nullptr;
	m_pBlendingAnimation = nullptr;

	m_fBlendTimer = 0;
	m_iFrameCounter = 0;

	m_pMyAppCBuffer = NULL;


	LoadShader();
}

void HierarchialParent::LoadHierarchyFromFile(const char * mFileName, const char* mEntity)
{
	std::ifstream inputStream;
	inputStream.open(mFileName);

	std::string line;

	while (!inputStream.eof())
	{
		std::getline(inputStream, line);
		std::string componentName, parentName, floatArray;

		componentName = line.substr(1, line.length() - 2);

		std::getline(inputStream, line);
		parentName = line.substr(1, line.length() - 2);

		std::getline(inputStream, line);
		floatArray = line;
		std::vector<double> positions = StringToDouble(Split(floatArray, ','));

		if (parentName == "")
		{
			AddHierarchyComponent(new HierarchialComponent(""), componentName);
		}
		else
		{
			string filepath = "./Resources/" + (std::string)mEntity + "/" + componentName + ".x";
			string meshTag = (std::string)mEntity + "_" + componentName;
			AddHierarchyComponent(new HierarchialComponent(parentName, MeshManager::GetInstance().LoadResources(filepath, meshTag)), componentName);
		}
	}

	inputStream.close();

}

HierarchialComponent* HierarchialParent::AddHierarchyComponent(HierarchialComponent * mComponent, const string& tag)
{
	//Add the component into the map with it's associated tag
	m_mHierarchyComponents.emplace(tag, mComponent);

	//Add the tag to a vector to hold the order of insertion
	//TODO Think of another way to handle this as having a map and a vector is a bit overkill
	m_vHierarchyOrder.push_back(tag);

	return mComponent;
}

HierarchialComponent * HierarchialParent::GetHiararchyComponentFromTag(char * tag)
{
	//If it can't be found in the map return a nullptr
	if (m_mHierarchyComponents.find(tag) == m_mHierarchyComponents.end())
	{
		return nullptr;
	}
	//Otherwise return the pointer
	else
	{
		return m_mHierarchyComponents.find(tag)->second;
	}
}

void HierarchialParent::UpdateHierarchy(bool mDebug)
{

	UpdateAnimations(mDebug);

	CalculateLocalMatrices();
	CalculateWorldMatrices();
}

void HierarchialParent::DrawHierarchy()
{
	for (std::map<std::string, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
	{
		it->second->Draw();
	}
}

XMFLOAT4 HierarchialParent::GetParentPosition()
{
	return m_v4LocalPos;
}

void HierarchialParent::SetActiveAnimation(int index)
{
	m_pActiveAnimation = &(m_mAnimations[index]);
}

void HierarchialParent::SetShaderForAll(Application::Shader * pShader)
{
	for (std::map<std::string, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
	{
		if (it->second->GetIsDrawable())
		{
			it->second->SetShader(pShader);
		}
	}
}

bool HierarchialParent::LoadShader()
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

	if (!CompileShadersFromFile(pDevice, "./Resources/Shaders/SpecularShader.hlsl", "VSMain", &pVS, &vs, g_aVertexDesc_Pos3fColour4ubNormal3f,
		g_vertexDescSize_Pos3fColour4ubNormal3f, &pIL, "PSMain", &pPS, &ps, aMacros))
	{
		return false; // false;
	}

	Application::s_pApp->CreateShaderFromCompiledShader(&m_shader, pVS, &vs, pIL, pPS, &ps);

	ps.FindCBuffer("MyApp", &m_psMyAppCBufferSlot);
	ps.FindFloat(m_psMyAppCBufferSlot, "g_cameraPosition", &m_cameraPosOffset);
	ps.FindFloat(m_psMyAppCBufferSlot, "g_frameCount", &m_framePosOffset);

	// Create a cbuffer, using the shader description to find out how
	// large it needs to be.
	m_pMyAppCBuffer = CreateBuffer(pDevice, ps.GetCBufferSizeBytes(m_psMyAppCBufferSlot), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL);

	// In this example we are sharing the constant buffer between both vertex and pixel shaders.
	// This is efficient since we only update one buffer. However we could define separate constant buffers for each stage.
	// Generally constant buffers should represent groups of variables that must be updated at the same rate.
	// So : we might have 'per execution' 'per frame', 'per draw' constant buffers.

	SetShaderForAll(&m_shader);
}

void HierarchialParent::DeleteShader()
{
	Release(m_pMyAppCBuffer);

	m_shader.Reset();
}

void HierarchialParent::SetBlendingAnimation(Animation * pBlend)
{
	if (m_pBlendingAnimation != NULL)
	{
		if (m_pBlendingAnimation != pBlend)
		{
			m_pActiveAnimation = m_pBlendingAnimation;
		}
	}
	m_pBlendingAnimation = pBlend;
	m_pBlendingAnimation->SetTime(m_pActiveAnimation->GetTimer(), m_pActiveAnimation->GetMaxTime());
}

void HierarchialParent::CalculateLocalMatrices()
{
	for (std::map<std::string, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
	{
		it->second->UpdateLocalMatrix();
	}

	//Todo add local rotation and scaling here 
	XMMATRIX mRot = XMMatrixRotationQuaternion(CalculateQuaternion(m_v4LocalRot));
	m_mLocalMatrix = mRot * XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4LocalPos));
}

void HierarchialParent::CalculateWorldMatrices()
{

	for (auto& tag : m_vHierarchyOrder)
	{
		HierarchialComponent* hc = m_mHierarchyComponents.find(tag)->second;
		if (hc->GetParentNode() != "")
		{
			std::string parentNode = hc->GetParentNode();
			XMMATRIX mWorldMatrix = hc->GetLocalMatrix() * (m_mHierarchyComponents.at(parentNode)->GetWorldMatrix());
			hc->SetWorldMatrix(&mWorldMatrix);
		}
		else
		{
			hc->SetWorldMatrix(&(hc->GetLocalMatrix() * m_mLocalMatrix));
		}
	}
}

void HierarchialParent::UpdateAnimations(bool mDebug)
{
	if (m_pActiveAnimation != nullptr)
	{
		m_pActiveAnimation->Update(mDebug);

		if (m_pBlendingAnimation != NULL)
		{
			m_pBlendingAnimation->Update(mDebug);


			if (m_fBlendTimer > m_pBlendingAnimation->GetBlendTime())
			{
				m_fBlendTimer = 0;
				m_pActiveAnimation = m_pBlendingAnimation;
				m_pBlendingAnimation = nullptr;
			}
		}


		for (std::map<std::string, HierarchialComponent*>::iterator it = m_mHierarchyComponents.begin(); it != m_mHierarchyComponents.end(); it++)
		{
			AnimationComponent* ac = m_pActiveAnimation->GetAnimationComponentByName(it->first);

			XMFLOAT4 activeAnimPosition = ac->GetCurrentPosition();
			XMFLOAT4 activeAnimRotation = ac->GetCurrentRotation();

			//If the blending animation has been set
			if (m_pBlendingAnimation != NULL)
			{
				AnimationComponent* blendAnimCompo = m_pBlendingAnimation->GetAnimationComponentByName(it->first);

				XMVECTOR blendAnimPosition = XMLoadFloat4(&blendAnimCompo->GetCurrentPosition()) - XMLoadFloat4(&activeAnimPosition);
				XMVECTOR blendAnimRotation = XMLoadFloat4(&blendAnimCompo->GetCurrentRotation()) - XMLoadFloat4(&activeAnimRotation);

				float t = m_fBlendTimer / m_pBlendingAnimation->GetBlendTime();

				blendAnimPosition *= t;
				blendAnimRotation *= t;

				blendAnimPosition += XMLoadFloat4(&activeAnimPosition);
				blendAnimRotation += XMLoadFloat4(&activeAnimRotation);

				XMFLOAT4 finalPos, finalRot;
				XMStoreFloat4(&finalPos, blendAnimPosition);
				XMStoreFloat4(&finalRot, blendAnimRotation);

				it->second->SetLocalPosition(finalPos);
				it->second->SetLocalRotationQuart(finalRot);
			}
			else
			{
				it->second->SetLocalPosition(activeAnimPosition);
				it->second->SetLocalRotationQuart(activeAnimRotation);
			}
		}

		if (m_pBlendingAnimation != NULL)
		{
			if (!mDebug)
				m_fBlendTimer += 0.0133f;
			else
			{
				m_iFrameCounter++;
				if (m_iFrameCounter > 60)
				{
					m_fBlendTimer += (0.0133f * 2);
					m_iFrameCounter = 0;
				}
			}
		}

	}
}

void HierarchialParent::UpdateShader(XMFLOAT3 camPos, float mFrameCount)
{
	ID3D11DeviceContext* pContext = Application::s_pApp->GetDeviceContext();

	if (m_pMyAppCBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		if (SUCCEEDED(pContext->Map(m_pMyAppCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map)))
		{
			// Set the buffer contents. There is only one variable to set in this case.
			// This method relies on the offset which has been found through Shader Reflection.
			SetCBufferFloat3(map, m_cameraPosOffset, D3DXVECTOR3(camPos.x, camPos.y, camPos.z));
			SetCBufferFloat(map, m_framePosOffset, mFrameCount);
			pContext->Unmap(m_pMyAppCBuffer, 0);
		}
	}

	// Bind the same constant buffer to any stages that use it.
	if (m_psMyAppCBufferSlot != -1)
	{
		pContext->PSSetConstantBuffers(m_psMyAppCBufferSlot, 1, &m_pMyAppCBuffer);
	}
}

XMVECTOR HierarchialParent::CalculateQuaternion(XMFLOAT4 rot)
{
	//Convert stored angle into radians
	double radX = XMConvertToRadians(rot.x);
	double radY = XMConvertToRadians(rot.y);
	double radZ = XMConvertToRadians(rot.z);

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

std::vector<std::string> HierarchialParent::Split(const std::string & txt, char ch)
{
	//Find position in string that contains specified character
	unsigned int pos = txt.find(ch);
	unsigned int startPos = 0;

	std::vector<std::string> vals;
	std::string t;

	//Loop while our pos value is valid
	while (pos != std::string::npos) {

		//Get the substring from the initial position to found pos
		t = txt.substr(startPos, pos - startPos);
		//Add string
		vals.push_back(t);

		//Update our start position
		startPos = pos + 1;

		//Try and find the specified character again. If it can't be found
		//find() will return string::npos
		pos = txt.find(ch, startPos);
	}

	// Add the last string
	vals.push_back(txt.substr(startPos, txt.size() - startPos));

	return vals;
}

std::vector<double> HierarchialParent::StringToDouble(const std::vector<std::string>& txt)
{
	std::vector<double> temp;

	for (auto& str : txt)
	{
		if (str[0] == ' ')
		{
			temp.push_back(std::stod(str.substr(1, str.length())) / 10.0);
		}
		else
		{
			temp.push_back(std::stod(str) / 10.0);
		}
	}

	return temp;
}
