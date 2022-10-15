// Disclaimer: I got most of this code from https://learnopengl.com which is a fantastic resource for starting out

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "testrender.hpp"

Triangle::Triangle() {
	vertices = {
		// positions			// colors
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f    // top 
	};
}

Rectangle::Rectangle() {
	vertices = {
	 0.5f,  0.5f, 0.0f,		// top right
	 0.5f, -0.5f, 0.0f,		// bottom right
	-0.5f, -0.5f, 0.0f,		// bottom left
	-0.5f,  0.5f, 0.0f };	// top left

	indices = {
		0, 1, 3,	// first triangle
		1, 2, 3 };	// second triangle
}

unsigned int testfunction(Triangle* triangle) {
	// Create a vertex array object that handles our vertex buffers
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Create a vertex buffer object that stores our vertices
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Copy our vertices into a buffer for OpenGL to use
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle->vertices), &(triangle->vertices), GL_STATIC_DRAW);

	// Tell OpenGL how our vertex data is organised
	// Positions are designated with location = 0 in the vertex shader
	// The total length of one block is 6 (3 for positions and 3 colors)
	// The positions occupy the first 3 positions (-> offset 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Positions are designated with location = 1 in the vertex shader
	// The total length of one block is 6 (3 for positions and 3 colors)
	// The positions occupy the second 3 positions (-> offset 3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}

unsigned int testfunction2(Rectangle* rectangle) {
	// Create an element buffer object that manages objects comprised of overlapping vertices
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Create a vertex array object that handles our vertex buffers
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Create a vertex buffer object that stores our vertices
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Copy our vertices into a vertex buffer for OpenGL to use
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle->vertices), &(rectangle->vertices), GL_STATIC_DRAW);

	// Copy our index array in an element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle->indices), &(rectangle->indices), GL_STATIC_DRAW);

	// Tell OpenGL how our vertex data is organised
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return VAO;
}

void testfunction3() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void updateColor(unsigned int shaderProgram) {
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "givenColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}