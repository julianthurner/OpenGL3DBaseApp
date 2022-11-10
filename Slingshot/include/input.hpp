#pragma once

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern void processKeyboardInput(GLFWwindow* window, float deltaTime);
extern void processMouse(GLFWwindow* window, double x, double y);
extern void processScrollwheel(GLFWwindow* window, double xoffset, double yoffset);