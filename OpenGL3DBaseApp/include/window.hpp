#pragma once

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
	static GLFWwindow& initialize();
	static void getMonitorScreenSize(unsigned int& width, unsigned int& height);
	static float getAspectRatio();
};