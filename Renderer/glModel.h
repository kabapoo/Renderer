#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

#include "Material.h"

class glModel
{
public:
	glModel();
	~glModel();

	int LoadOBJ(const char* path);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;

	void SetTransform();
	glm::mat4 GetTransform();
	void SetUniform();

	void Start(GLuint id);
	void Update();

private:
	glm::mat4 transformMatrix;
	GLuint ShaderID;
	GLuint ModelMatrixID;

	int isLoaded;
};