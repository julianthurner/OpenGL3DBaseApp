#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders.hpp"
#include "camera.hpp"

extern void initializeCamera(Camera* camera);
extern void processKeyboardInput(GLFWwindow* window, Shader* shader, float deltaTime);
extern void processMouse(GLFWwindow* window, double x, double y);
extern void processScrollwheel(GLFWwindow* window, double xoffset, double yoffset);