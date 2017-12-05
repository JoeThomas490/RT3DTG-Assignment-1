#pragma once

#include "Application.h"

#include "CommonMesh.h"

#include <map>
#include <vector>

typedef std::string string;

class MeshManager
{
public:

	~MeshManager();

	CommonMesh* LoadResources(const string& fileName, string tag);
	CommonMesh* GetMeshFromTag(string tag);

	void DeleteResource(string tag);

private:

	std::map<string, int> m_mMeshLookup;
	std::vector<CommonMesh*> m_vMeshes;

	MeshManager();

public:

	static MeshManager& GetInstance()
	{
		static MeshManager* instance = new MeshManager();
		return *instance;
	}

};

