#include "glLog.h"

glLog::glLog()
{
	
}

glLog::~glLog()
{

}

void glLog::Start()
{
	assert(restartLog());
	log("Starting GLFW\n%s\n", glfwGetVersionString());
	glfwSetErrorCallback(glfw_error_callback);
	log_gl_params();
}

glLog& glLog::getInstance()
{
	static glLog instance;
	return instance;
}

void glLog::glfw_error_callback(int error, const char* description)
{
	getInstance().logerr("GLFW ERROR: code %i msg: %s\n", error, description);
}

int glLog::restartLog()
{
	FILE* fp;
	fopen_s(&fp, GL_LOG_FILE, "w");
	if (!fp)
	{
		fprintf_s(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return 0;
	}
	time_t now = time(NULL);
	char* date = ctime(&now);
	fprintf_s(fp, "GL_LOG_FILE log. local time %s\n", date);
	fclose(fp);
	return 1;
}

int glLog::log(const char* msg, ...)
{
	va_list argptr;
	FILE* fp;
	fopen_s(&fp, GL_LOG_FILE, "a");
	if (!fp)
	{
		fprintf_s(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return 0;
	}
	va_start(argptr, msg);
	vfprintf_s(fp, msg, argptr);
	va_end(argptr);
	fclose(fp);
	return 1;
}

int glLog::logerr(const char* msg, ...)
{
	va_list argptr;
	FILE* fp;
	fopen_s(&fp, GL_LOG_FILE, "a");
	fopen_s(&fp, GL_LOG_FILE, "a");
	if (!fp)
	{
		fprintf_s(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return 0;
	}
	va_start(argptr, msg);
	vfprintf_s(fp, msg, argptr);
	va_end(argptr);
	va_start(argptr, msg);
	vfprintf_s(stderr, msg, argptr);
	va_end(argptr);
	fclose(fp);
	return 1;
}

void glLog::log_gl_params()
{
	GLenum params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};
	const char* names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};
	log("GL Context Params:\n");
	char msg[256];
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv(params[i], &v);
		log("%s %i\n", names[i], v);
	}
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	log("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	log("%s %u\n", names[11], (unsigned int)s);
	log("-----------------------------\n");
}