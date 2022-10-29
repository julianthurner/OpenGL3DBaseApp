#pragma once

#include <array>

#include "shaders.hpp"
#include "camera.hpp"

class Triangle {
public:
	std::array<float, 18> vertices;

	Triangle();
};

class Rectangle {
public:
	std::array<float, 32> vertices;
	std::array<int, 6> indices;
	std::array<float, 6> textureCoordinates;
	unsigned int texture1ID;
	unsigned int texture2ID;

	Rectangle(const char* texture1Path, const char* texture2Path);
};

class Cube {
public:
	std::array<float, 180> vertices;
	unsigned int texture1ID;
	unsigned int texture2ID;

	Cube(const char* texture1Path, const char* texture2Path);
};

extern unsigned int testfunction(Triangle* triangle);
extern unsigned int testfunction2(Rectangle* rectangle);
extern void testfunction3();
extern void updateColor(unsigned int shaderProgram);
extern void testfunction4(Rectangle* rectangle);
extern void testfunction5();
extern void testfunction6(Shader* shader, float delta);
extern void testfunction7(Shader* shader, Camera* cam);
extern unsigned int testfunction8(Cube* cube);
extern void testfunction9(Cube* cube);
extern void testfunction10(Shader* shader, unsigned int i);