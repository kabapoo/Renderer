#pragma once

#include <stdio.h>
#include <GLFW\glfw3.h>

class FrameRateCounter
{
public:
	FrameRateCounter();
	~FrameRateCounter();

	void update(GLFWwindow* window);
};