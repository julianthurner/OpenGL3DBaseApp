#pragma once

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern float aspectRatio;

extern GLFWwindow& initializeWindow();
extern void getScreenSize(int* width, int* height);