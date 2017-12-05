#pragma once

#include "Application.h"

#include "HierarchialParent.h"
#include "HierarchialComponent.h"

#include "MeshManager.h"

//ToDo Add colour to hierarchy.txt and also load that from file

__declspec(align(16))class Robot : public HierarchialParent
{
public:
	Robot();
	Robot(float fX, float fY, float fZ, float fRotY);
	virtual ~Robot() = default;

	void Update(bool mDebug);
	void Draw(XMFLOAT3 camPos, float mFrameCount);
private:

private:


};

