#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class glShader
{
public:
	glShader();
	~glShader();

	GLuint Start(const char * vertex_file_path, const char * fragment_file_path);
	GLuint LoadShader(const char * vertex_file_path, const char * fragment_file_path);
	void PrintInfo(GLuint id);
	void PrintProgramInfo(GLuint programID);
	int isValid(GLuint programID);
	GLuint GetID() const;

private:
	GLuint id;
	GLuint ProjectionMatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
};