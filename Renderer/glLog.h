#pragma once
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "Component.h"

#define GL_LOG_FILE "gl.log"

class glLog : public Component
{
public:
	glLog();
	~glLog();

	void Start();
	
	static glLog& getInstance();
	static void glfw_error_callback(int error, const char* description);

	int restartLog();
	int log(const char* msg, ...);
	int logerr(const char* msg, ...);
	
	void log_gl_params();

private:

};
