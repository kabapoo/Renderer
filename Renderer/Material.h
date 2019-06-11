#pragma once
#include <iostream>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/transform.hpp>

class Material
{
public:
	Material();
	~Material();

	virtual void Start() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void SetUniform() = 0;

	void SetShaderID(GLuint id);

	int type;
	

protected:
	// uniform variable handles
	GLuint ShaderID;
	GLuint MaterialTypeID;

	int isUpdatable;
};