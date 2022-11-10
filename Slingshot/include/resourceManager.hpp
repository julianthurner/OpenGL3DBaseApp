#pragma once

#include <vector>

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "shaders.hpp"

extern void initializeResourceManager(GLFWwindow& window);
extern void render();
extern Camera& giveCamera();
extern std::vector<Shader>& giveShaders();