#pragma once

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Input {
public:
	static void processKeyboardInput(GLFWwindow& window, const float deltaTime);
	
	// Callbacks
	static void processMouse(GLFWwindow* window, double x, double y);
	static void processScrollwheel(GLFWwindow* window, double xOffset, double yOffset);
};