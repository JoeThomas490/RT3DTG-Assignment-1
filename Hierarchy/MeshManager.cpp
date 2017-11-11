#include "MeshManager.h"


MeshManager* MeshManager::s_pMeshManager = nullptr;


MeshManager::MeshManager()
{
	s_pMeshManager = this;
}

MeshManager::~MeshManager()
{
	for (std::map<string, CommonMesh*>::iterator it = m_mMeshes.begin(); it != m_mMeshes.end(); it++)
	{
		delete it->second;
	}
}

CommonMesh * MeshManager::LoadResources(char* fileName, string tag)
{
	//If it can't be found in the map
	if (m_mMeshes.find(tag) == m_mMeshes.end())
	{
		CommonMesh* mesh = CommonMesh::LoadFromXFile(Application::s_pApp, fileName);
		m_mMeshes.emplace(tag, mesh);
		return mesh;
	}
	//Otherwise return the pointer
	else
	{
		return m_mMeshes.at(tag);
	}
}

CommonMesh* MeshManager::GetMeshFromTag(string tag)
{
	return m_mMeshes.at(tag);
}
