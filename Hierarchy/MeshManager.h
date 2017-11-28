#pragma once

#include "Application.h"

#include "CommonMesh.h"

#include <map>
#include <string>

typedef std::string string;

class MeshManager
{
public:
	~MeshManager();

	CommonMesh* LoadResources(const string& fileName, string tag);
	CommonMesh* GetMeshFromTag(string tag);

	void DeleteResource(string tag);

private:

	std::map<string, CommonMesh*> m_mMeshes;

	MeshManager();


public:

	static MeshManager& GetInstance()
	{
		static MeshManager* instance = new MeshManager();
		return *instance;
	}

};

