#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern float aspectRatio;

extern GLFWwindow* initializeWindow(int* resCode);
extern void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void getScreenSize(int* width, int* height);
