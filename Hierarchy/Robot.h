#pragma once

#include "Application.h"

#include "HierarchialParent.h"
#include "HierarchialComponent.h"

#include "MeshManager.h"

__declspec(align(16))class Robot : public HierarchialParent
{
public:
	Robot();
	Robot(float fX, float fY, float fZ, float fRotY);
	~Robot() = default;

	void Update();
	void Draw();

	static void LoadResources();

private:

	void AddComponents();

private:


};

