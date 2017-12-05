#include "MeshManager.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
	for (std::map<string, int>::iterator it = m_mMeshLookup.begin(); it != m_mMeshLookup.end(); it++)
	{
		delete m_vMeshes[it->second];
	}
}

CommonMesh * MeshManager::LoadResources(const string& mFileName, string tag)
{
	//Convert string filename to c string
	const char* fileName = mFileName.c_str();

	//If it can't be found in the map then it's a new mesh
	if (m_mMeshLookup.find(tag) == m_mMeshLookup.end())
	{
		CommonMesh* mesh = CommonMesh::LoadFromXFile(Application::s_pApp, fileName);
		m_vMeshes.push_back(mesh);
		m_mMeshLookup.emplace(tag, m_vMeshes.size() - 1);
		return mesh;
	}

	//Otherwise return the pointer
	else
	{
		int index = m_mMeshLookup.at(tag);
		return m_vMeshes[index];
	}
}

CommonMesh* MeshManager::GetMeshFromTag(string tag)
{

	int indx = m_mMeshLookup.at(tag);
	
	if (indx > 0 && indx < m_vMeshes.size())
	{
		return m_vMeshes[indx];
	}
	else
	{
		return nullptr;
	}
}

void MeshManager::DeleteResource(string tag)
{
	int indx = m_mMeshLookup.at(tag);

	if (indx > 0 && indx < m_vMeshes.size())
	{
		delete m_vMeshes[indx];
	}
}
