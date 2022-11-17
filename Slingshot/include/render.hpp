#pragma once

#include <array>
#include <string>
#include <vector>

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders.hpp"
#include "camera.hpp"

class Triangle {
private:
	unsigned int VAO;

	void initializeVAO(std::array<float, 18> vertices);
public:
	Triangle();
	Triangle(const std::array<float, 18> givenVertices);

	void render();
};

class Rectangle {
private:
	unsigned int VAO;
	unsigned int texture1ID;
	unsigned int texture2ID;

	void initializeTextures(std::string& texture1Path, std::string& texture2Path);
	void initializeVAO();
public:
	Rectangle(std::string texture1Path, std::string texture2Path);

	void render();
};

class Cube {
private:
	unsigned int VAO;
	unsigned int texture1ID;
	unsigned int texture2ID;

	void initializeTextures(std::string& texture1Path, std::string& texture2Path);
	void initializeVAO();
	void updateModelMatrix(Shader& shader, glm::vec3 cubePosition);
public:
	Cube(std::string texture1Path, std::string texture2Path);

	void renderMultiple(Shader& shader, std::vector<glm::vec3>& cubePositions);
};

class Render {
public:
	static void initialize(GLFWwindow& window);
	static void clearWindow();
	static void updateBlendValue(Shader& shader, float delta);
	static void updateMatrices(Shader& shader);
};