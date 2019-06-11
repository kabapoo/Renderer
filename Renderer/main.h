#pragma once
#include "glRenderer.h"

using namespace glm;

class MainFrame
{
public:
	MainFrame();
	~MainFrame();

	void run();
	int InitGL();
	int CreateWindow(int mode, char* windowName);
	void getVerInfo();
	void MainLoop();

	static void glfw_window_size_callback(GLFWwindow* window, int width, int height);

	glRenderer Renderer;
	//glModule *Module;
	GLFWwindow *pWindow;

private:
	// window size
	int window_width;
	int window_height;

	// viewport size
	int vp_width;
	int vp_height;
};