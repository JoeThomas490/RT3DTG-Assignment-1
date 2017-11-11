#pragma once

#include "Application.h"

#include "CommonMesh.h"

#include <map>
#include <string>

typedef std::string string;

class MeshManager
{
public:
	static MeshManager* s_pMeshManager;

public:
	MeshManager();
	~MeshManager() = default;

	CommonMesh* LoadResources(char* fileName, string tag);
	CommonMesh* GetMeshFromTag(string tag);
private:

	std::map<string, CommonMesh*> m_mMeshes;

};

