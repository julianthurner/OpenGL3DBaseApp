#pragma once

#include <array>

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

	void initializeTextures(const char* texture1Path, const char* texture2Path);
	void initializeVAO();
public:
	Rectangle(const char* texture1Path, const char* texture2Path);

	void render();
};

class Cube {
private:
	unsigned int VAO;
	unsigned int texture1ID;
	unsigned int texture2ID;

	void initializeTextures(const char* texture1Path, const char* texture2Path);
	void initializeVAO();
	void updateModelMatrix(Shader* shader, glm::vec3 cubePosition);
public:
	Cube(const char* texture1Path, const char* texture2Path);

	void renderMultiple(Shader* shader, glm::vec3* cubePositions);
};

extern void initializeRender();
extern void clearWindow();
extern void updateBlendValue(Shader* shader, float delta);
extern void updateMatrices(Shader* shader, Camera* cam);