#pragma once

#include <string>
#include <vector>

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shaders.hpp"

class Triangle {
private:
	unsigned int VAO_ID;

	void initializeVAO(const std::vector<float>& vertices);
public:
	Triangle();
	Triangle(const std::vector<float>& vertices);

	void render();
};

class Rectangle {
private:
	unsigned int VAO_ID;
	unsigned int texture1ID;
	unsigned int texture2ID;

	void initializeTextures(const std::string& texture1Path, const std::string& texture2Path);
	void initializeVAO();
public:
	Rectangle(const std::string& texture1Path, const std::string& texture2Path);

	void render();
};

class Cube {
private:
	unsigned int VAO_ID;
	unsigned int texture1ID;
	unsigned int texture2ID;

	void initializeTextures(const std::string& texture1Path, const std::string& texture2Path);
	void initializeVAO();
	void calculateModelMatrix(const Shader& shader, const glm::vec3& cubePosition);
public:
	Cube(const std::string& texture1Path, const std::string& texture2Path);

	void renderMultiple(const Shader& shader, const std::vector<glm::vec3>& cubePositions);
};

class Plane {
private:
	unsigned int VAO_ID;
	
	void initializeVAO();
public:
	Plane();

	void render();
};

class Render {
public:
	static void initialize(GLFWwindow& window);
	static void clearWindow();
	static void updateBlendValue(const Shader& shader, const float delta);
};