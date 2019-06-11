#include "main.h"
#include <iostream>

using namespace std;

MainFrame::MainFrame()
{
	window_width = 640;
	window_height = 480;
	vp_width = 640;
	vp_height = 480;
}

MainFrame::~MainFrame()
{

}

void MainFrame::glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
	static MainFrame main_inst;
	main_inst.window_width = width;
	main_inst.window_height = height;
}

int MainFrame::InitGL()
{
	if (!glfwInit())
	{
		wcerr << L"glfw: Initialising Failed.\n";
		return 0;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);  // 4x Anti-aliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window = glfwCreateWindow(1920, 1080, "Main Window", NULL, NULL);
	CreateWindow(0, "Main Window");
	if (!pWindow)
	{
		wcerr << L"ERROR: could not open window with GLFW3\n";
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(pWindow);  // for GLEW
	
	// GLEW extension handler
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		wcerr << L"Failed to initialise GLEW\n";
		return 0;
	}

	getVerInfo();

	// When sticky keys mode is enabled, the pollable state of a key will remain GLFW_PRESS until the state of that key is polled with glfwGetKey
	glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetWindowSizeCallback(pWindow, glfw_window_size_callback);

	glViewport(0, 0, vp_width, vp_height);

	return 1;
}

int MainFrame::CreateWindow(int mode, char* windowName)
{
	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* vmode = glfwGetVideoMode(mon);
	window_width = vmode->width - 160;
	window_height = vmode->height - 90;
	pWindow = glfwCreateWindow(window_width, window_height, windowName, NULL, NULL);
	vp_width = window_width;
	vp_height = window_height;

	return 1;
}

void MainFrame::getVerInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);		// get renderer string
	const GLubyte* version = glGetString(GL_VERSION);		// version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
}

void MainFrame::MainLoop()
{
	while (!glfwWindowShouldClose(pWindow) && glfwGetKey(pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer.Draw();
		
		// update other events like input handling
		glfwPollEvents();

		// draw
		glfwSwapBuffers(pWindow);

		// Framerate counter
		Renderer.modules.pFRC->update(pWindow);
	}
}

void MainFrame::run()
{
	InitGL();
	Renderer.Start();
	Renderer.Options();
	MainLoop();
}

int main(int argc, char* argv[])
{
	MainFrame _main;
	_main.run();

	return 0;
}
