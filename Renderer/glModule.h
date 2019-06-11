#pragma once
#include <GL\glew.h>

#include "glLog.h"
#include "FrameRateCounter.h"
#include "glModel.h"
#include "glShader.h"
#include "Camera.h"
#include "ILight.h"
#include "CookTorrance.h"
#include "MERL.h"
#include "Component.h"

using namespace std;

class glModule
{
public:
	glModule();
	~glModule();

	void Start();

	void AddCamera();
	void AddModel();
	void SelectMaterial(int type);

	void Update();

	glLog* pLog;
	FrameRateCounter* pFRC;
	glShader* pShader;
	Camera* pCamera;
	ILight* pLight;
	Material* pMaterial;

	CookTorrance ctMaterial;
	MERL merlMaterial;
	glModel simpleSphere;

private:
	GLuint ShaderID;
};
