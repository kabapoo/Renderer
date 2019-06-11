#include "glModule.h"

glModule::glModule()
{
	pLog = new glLog;
	pFRC = new FrameRateCounter;
	pShader = new glShader;
}

glModule::~glModule()
{
	delete pLog;
	delete pFRC;
	delete pShader;
	if (pCamera)
		delete pCamera;
	if (pLight)
		delete pLight;
}

void glModule::Start()
{
	pLog->Start();
	//ShaderID = pShader->Start("SimpleVertexShader.vert", "SimpleFragmentShader.frag");
	ShaderID = pShader->Start("MultipleLightShader.vert", "MultipleLightShader.frag");
	// load sphere and connect to shader
	simpleSphere.LoadOBJ("Sphere2.obj");
	simpleSphere.Start(ShaderID);
	// Create and initialise materials and then connect to the shader
	ctMaterial.Init();
	ctMaterial.SetShaderID(ShaderID);
	merlMaterial.run("..\\..\\MERLDatabase\\materials\\green-metallic-paint.binary");
	merlMaterial.SetShaderID(ShaderID);
	// Add a basic camera and lights
	AddCamera();
	pCamera->Start(ShaderID);
	pLight = new ILight(ShaderID);		// Light Interface
	pLight->AddLight(0, 0, glm::vec3(-1.5f, 1.7f, 4.0f), glm::vec3(1.0f, 1.0f, 1.0f), 7.0f);
	//pLight->AddLight(1, 1, glm::vec3(1.5f, 1.5f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f);
	//pLight->AddLight(2, 0, glm::vec3(1.5f, -3.5f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);
	pLight->SetUniforms();

	// Select one of the material and start(make this material updatable)
	simpleSphere.colors = merlMaterial.SetColorBuffer(simpleSphere.vertices, simpleSphere.normals, simpleSphere.colors, pLight->GetPosition(0), pCamera->GetPosition());
	std::cout << "colorbuffer size : " << simpleSphere.colors.size() << std::endl;
	SelectMaterial(2);		// cook torrance : 1 , MERL : 2
	pMaterial->Start();
}

void glModule::AddCamera()
{
	pCamera = new Camera;
}

void glModule::AddModel()
{

}

void glModule::SelectMaterial(int type)
{
	switch (type)
	{
	case 1:
		pMaterial = &ctMaterial;
		pMaterial->type = 1;
		break;
	case 2:
		pMaterial = &merlMaterial;
		pMaterial->type = 2;
		break;
	}
	
}

void glModule::Update()
{
	simpleSphere.Update();
	pCamera->Update();
	pLight->Update();
	pMaterial->Update();
}