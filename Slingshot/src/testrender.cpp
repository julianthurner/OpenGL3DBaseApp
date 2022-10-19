// Disclaimer: I got most of this code from https://learnopengl.com which is a fantastic resource for starting out

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Seems strange, but this is the correct way of including stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

#include "testrender.hpp"

float blendValue;

Triangle::Triangle() {
	vertices = {
		// positions			// colors
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f    // top 
	};
}

Rectangle::Rectangle(const char* texture1Path, const char* texture2Path) {
	vertices = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	indices = {
		0, 1, 3,	// first triangle
		1, 2, 3,	// second triangle
	};	

	textureCoordinates = {
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};

	/* Generating the texture */
	// Load the image from file
	int width, height, numberOfColorChannels;
	unsigned char* imageData1 = stbi_load(texture1Path, &width, &height, &numberOfColorChannels, 0);
	
	// Generate a new texture. glGenTextures takes the number of textures to create as first argument
	glGenTextures(1, &texture1ID);
	glBindTexture(GL_TEXTURE_2D, texture1ID);

	// Set texture wrapping to "Mirror" on both axes (-> S-axe and T-axe which correspond to x and y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Set texture filtering to "Linear"
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set mipmapping to "Linear"
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	// Fill the created texture with the previously loaded image data
	if (imageData1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture 1\n";
	}

	// Free the image from memory
	stbi_image_free(imageData1);

	// Repeat the same process for the second texture
	unsigned char* imageData2 = stbi_load(texture2Path, &width, &height, &numberOfColorChannels, 0);
	glGenTextures(1, &texture2ID);
	glBindTexture(GL_TEXTURE_2D, texture2ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	if (imageData2) {
		// This time the image has an alpha channel, thus use GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture 2\n";
	}
	stbi_image_free(imageData2);
}

// The test functions should be moved into the object creation later on
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
	// Colors are designated with location = 1 in the vertex shader
	// The total length of one block is 6 (3 for positions and 3 colors)
	// The colors occupy the second 3 positions (-> offset 3)
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
	// Vertexc Positions are designated with location = 0 in the vertex shader
	// The total length of one block is 8 (3 for vertex positions, for 3 colors and 2 for texture mapping positions)
	// The vertrex positions occupy the first 3 positions (-> offset 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Colors are designated with location = 1 in the vertex shader
	// The total length of one block is still 8
	// The colors occupy positions 4 through 6 (-> offset 3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture mapping positions are designated with location = 2 in the vertex shader
	// The total length of one block is still 8
	// The texture mapping positions occupy positions 7 through 8 (-> offset 6)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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

void testfunction4(Rectangle* rectangle) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rectangle->texture1ID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rectangle->texture2ID);
}

void testfunction5() {
	stbi_set_flip_vertically_on_load(true);
}

void testfunction6(Shader* shader, float delta) {
	if (blendValue) {
		blendValue += delta;
		if (blendValue > 1.0f) {
			blendValue = 1.0f;
		}
		else if (blendValue < 0.0f) {
			blendValue = 0.0f;
		}

	}
	else {
		if (delta > 0) {
			blendValue = delta;
		}
	}

	shader->setFloat("blendValue", blendValue);
}

void testfunction7(Shader* shader) {
	glm::mat4 transformMatrix = glm::mat4(1.0f); // Creates identity matrix
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.5f, -0.5, 0.0f));
	transformMatrix = glm::rotate(transformMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	
	shader->setMat4("transformMatrix", transformMatrix);
}

void testfunction8(Shader* shader) {
	glm::mat4 transformMatrix = glm::mat4(1.0f); // Creates identity matrix
	transformMatrix = glm::translate(transformMatrix, glm::vec3(-0.5f, 0.5, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(sin(glfwGetTime()) / 2 + 0.5, sin(glfwGetTime()) / 2 + 0.5, 0.0f));
	

	shader->setMat4("transformMatrix", transformMatrix);
}