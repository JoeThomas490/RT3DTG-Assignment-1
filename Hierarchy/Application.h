#ifndef APPLICATION_H
#define APPLICATION_H

#define WIN32_LEAN_AND_MEAN

#include <assert.h>
#include <string>
#include <vector>

#include <stdio.h>
#include <windows.h>
#include <d3d11.h>

#include "CommonApp.h"
#include "CommonMesh.h"

#include "tinyxml\tinyxml.h"



class Aeroplane;
class HeightMap;	
class Bullet;
class Robot;
class CubeMap;
class MeshManager;
class AnimationLoader;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class Application : public CommonApp
{
  public:
	static Application* s_pApp;
	
  protected:
	bool HandleStart();
	void HandleStop();
	void HandleUpdate();
	void HandleRender();

	void HandleCollision();
	void HandleDebug();
	void HandleSpawnBullets();

	void HandleCameraUpdate();

	void LoadXML();

private:

	void SelectAnimation();

	float GetLengthBetweenEntities(XMFLOAT4 a, XMFLOAT4 b);

  private:
	float m_rotationAngle;
	float m_cameraZ;

	bool m_bWireframe;
	bool m_bDebugAnimations;

	float m_fFrameCount;

	int m_cameraState;

	XMFLOAT3 m_cameraPos;

	HeightMap* m_pHeightMap;

	Aeroplane* m_pAeroplane;
	Robot* m_pRobot;

	CubeMap* m_pSkyBox;

	std::vector<Robot*> m_pRobots;
};

#endif
