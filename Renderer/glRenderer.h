#pragma once
#include <GL\glew.h>
#include "glModule.h"
#include "glModel.h"
#include "glShader.h"

class glRenderer
{
public:
	glRenderer();
	~glRenderer();

	void Start();
	void Options();
	void Draw();

	void CreateVBO();
	void CreateVAO();

	// VBO IDs
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint colorBuffer;
	GLuint normalBuffer;
	
	// IDs
	GLuint vaoID;

	// Module
	glModule modules;

private:
};