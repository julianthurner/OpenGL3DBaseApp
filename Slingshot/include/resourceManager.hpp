#pragma once

#include <vector>

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "shaders.hpp"

class ResourceManager {
public:
	static void initialize(GLFWwindow& window);
	static void render();
	static Camera& giveCamera();
	static std::vector<Shader>& giveShaders();
	static void setViewMatrix(glm::mat4 viewMatrix);
	static void setProjectionMatrix(glm::mat4 projectionMatrix);
};