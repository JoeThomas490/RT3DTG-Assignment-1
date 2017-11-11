#include "MeshManager.h"


MeshManager* MeshManager::s_pMeshManager = nullptr;


MeshManager::MeshManager()
{
	s_pMeshManager = this;
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
