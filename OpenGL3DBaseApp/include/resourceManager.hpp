#pragma once

#include <vector>

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera.hpp"
#include "shaders.hpp"

class ResourceManager {
public:
	static void initialize(GLFWwindow& window);
	static void render();
	static Camera& giveCamera();
	static const std::vector<Shader>& giveShaders();
	static void setViewMatrix(const glm::mat4& viewMatrix);
	static void setProjectionMatrix(const glm::mat4& projectionMatrix);
};