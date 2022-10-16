#pragma once

#include <array>

class Triangle {
public:
	std::array<float, 18> vertices;

	Triangle();
};

class Rectangle {
public:
	std::array<float, 12> vertices;
	std::array<int, 6> indices;

	Rectangle();
};

extern unsigned int testfunction(Triangle* triangle);
extern unsigned int testfunction2(Rectangle* rectangle);
extern void testfunction3();
extern void updateColor(unsigned int shaderProgram);