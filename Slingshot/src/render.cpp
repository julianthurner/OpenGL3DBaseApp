// Disclaimer: I got most of this code from https://learnopengl.com which is a fantastic resource for starting out

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

// Seems strange, but this is the correct way of including stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

#include "render.hpp"
#include "input.hpp"
#include "window.hpp"
#include "resourceManager.hpp"

//** Private **//
float blendValue;

void loadTexture(const std::string& texturePath, unsigned int& textureID) {
	// Load the image from file
	int width, height, numberOfColorChannels;
	int rgbType = 0;
	// 1st argument takes a const char* to the filepath
	// 2nd, 3rd and 4th argument take pointers to store width, height and number of color channels in
	// 5th argument can be used to force number of 8-bit components per pixel. We don't need that, so we leave it at 0
	// As much as I hate it, stbi_load actually requires you to store a raw pointer, there's no way around it
	// Otherwise you won't be able to call stbi_image_free later on
	unsigned char* imageData = stbi_load(texturePath.c_str(), &width, &height, &numberOfColorChannels, 0);
	// 3 color channels means no alpha channel, 4 means there is one
	switch (numberOfColorChannels) {
	case 3:
		rgbType = GL_RGB;
		break;
	case 4:
		rgbType = GL_RGBA;
		break;
	default:
		std::cout << "Error: Image's color channels were not properly recognized." << "\n";
		std::cout << "Image has " << numberOfColorChannels << " color channels." << std::endl;
	}

	// Generate a new texture. First argument is the number of textures to create
	glGenTextures(1, &textureID);
	// Tells OpenGL which texture we are currently working with. First argument specifies type of texture, second one takes the ID
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set texture wrapping to "Mirror" on both axes (-> S-axe and T-axe which correspond to x and y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Set texture filtering to "Linear"
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set mipmapping to "Linear"
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Fill the created texture with the previously loaded image data
	if (imageData) {
		// First argument is the type of the texture, second one is for setting mipmap levels manually which we don't want to do -> leave at 0
		// Third argument tells OpenGL the format the texture is to be stored in, 4th and 5th argument should be self-explanatory
		// 6th argument has to be 0 at all times (specifies border width; legacy)
		// 7th argument specifies the format of the source image (in this case RGB without alpha channel, so GL_RGB)
		// 8th argument specifies the datatype of the source image; since we stored it as unsigned char (-> byte), this has to be GL_UNSIGNED_BYTE
		// 9th argument is a pointer to the actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgbType, GL_UNSIGNED_BYTE, imageData);
		// This function automatically creates mipmaps for the texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	// Free the image from memory
	stbi_image_free(imageData);
}

//** Public **//
Triangle::Triangle() {
	std::array<float, 18> vertices = {
		// positions			// colors
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f    // top 
	};
	initializeVAO(vertices);
}

Triangle::Triangle(const std::array<float, 18> givenVertices) {
	std::array<float, 18> vertices = givenVertices;
	initializeVAO(vertices);
}

void Triangle::initializeVAO(std::array<float, 18> vertices) {
	//* Create a vertex array object that tells OpenGL how to interpret VBO data (see down below)
	// First parameter defines number of VAOs created, seconds takes a pointer to the VAO ID address
	glGenVertexArrays(1, &VAO);

	//* Create a vertex buffer object that stores our data
	// Since we won't change the vertex data, there is no need for us to store the VBO ID permanently
	unsigned int VBO;
	// First parameter defines number of VBOs created, seconds takes a pointer to the VBO ID address
	glGenBuffers(1, &VBO);

	//* Copy our vertices into the VBO
	// This function tells OpenGL which VAO we are currently working with
	glBindVertexArray(VAO);
	// Same for the VBO
	// First parameter tells OpenGL which type of buffer it has to deal with, second parameter is the VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Here the actual data copying happens
	// Static draw specifies that the data is set only once and used many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	//* Tell OpenGL how our vertex data is organised
	// First argument is the location where the set of data is stored in the vertex shaders
	// Second argument is the size of the specified vertex attribute
	// Third argument specifies the data type of the specified vertex attribute
	// 4th argument specifies if we want the data to be normalized to [-1;1] / [0;1], which we don't need, so we use GL_FALSE
	// 5th argument defines the stride, which means the space between consecutive vertex attributes
	// 6th argument defines the offset, which means after which space the specified attribute data is stored
	// Vertex positions are designated with location = 0 in the vertex shader and are comprised of 3 values
	// The total length of one block is 6 (3 for positions and 3 colors)
	// The vertex positions occupy the first 3 positions (-> offset 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// After each specification with glVertexAttribPointer, we need to call glEnableVertexAttribArray
	// This is because vertex attributes are disabled by default
	// The functions takes the location we previously used as its argument
	glEnableVertexAttribArray(0);
	// Colors are designated with location = 1 in the vertex shader
	// The total length of one block is still 6 (3 for positions and 3 colors)
	// The colors occupy the second 3 positions (-> offset 3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Triangle::render() {
	// Tells OpenGL that it is working with this triangle's VAO
	// Remember that all that OpenGL needs is an VAO; it contains the necessary pointers to VBOs
	glBindVertexArray(VAO);
	// Draw the triangle
	// First argument is the type of object to draw
	// The second one is the starting index of the array (so in this case 0)
	// The third one is the amount of vertices of the object
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

Rectangle::Rectangle(const std::string& texture1Path, const std::string& texture2Path) {
	initializeTextures(texture1Path, texture2Path);
	initializeVAO();
}

void Rectangle::initializeTextures(const std::string& texture1Path, const std::string& texture2Path) {
	loadTexture(texture1Path, texture1ID);
	loadTexture(texture2Path, texture2ID);
}

void Rectangle::initializeVAO() {
	std::array<float, 32> vertices = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	std::array<unsigned int, 6> indices = {
		0, 1, 3,	// first triangle
		1, 2, 3,	// second triangle
	};

	std::array<float, 6> textureCoordinates = {
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};

	// Create an element buffer object -> manages objects comprised of overlapping vertices
	// Since we won't change the index data, there is no need for us to store the EBO ID permanently
	unsigned int EBO;
	// First parameter defines number of EBOs created, seconds takes a pointer to the EBO ID address
	glGenBuffers(1, &EBO);

	//* Create a vertex array object that tells OpenGL how to interpret VBO data (see down below)
	// First parameter defines number of VAOs created, seconds takes a pointer to the VAO ID address
	glGenVertexArrays(1, &VAO);

	//* Create a vertex buffer object that stores our data
	// Since we won't change the vertex data, there is no need for us to store the VBO ID permanently
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//* Copy our vertices into a vertex buffer for OpenGL to use
	// This function tells OpenGL which VAO we are currently working with
	glBindVertexArray(VAO);
	// Same for the VBO
	// First parameter tells OpenGL which type of buffer it has to deal with, second parameter is the VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Here the actual data copying happens
	// Static draw specifies that the data is set only once and used many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	//* Copy our index array in an element buffer for OpenGL to use
	// Same as for the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	//* Tell OpenGL how our vertex data is organised
	// First argument is the location where the set of data is stored in the vertex shaders
	// Second argument is the size of the specified vertex attribute
	// Third argument specifies the data type of the specified vertex attribute
	// 4th argument specifies if we want the data to be normalized to [-1;1] / [0;1], which we don't need, so we use GL_FALSE
	// 5th argument defines the stride, which means the space between consecutive vertex attributes
	// 6th argument defines the offset, which means after which space the specified attribute data is stored
	// Vertex positions are designated with location = 0 in the vertex shader
	// The total length of one block is 8 (3 for vertex positions, 3 for colors and 2 for texture mapping positions)
	// The vertex positions occupy the first 3 positions (-> offset 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// After each specification with glVertexAttribPointer, we need to call glEnableVertexAttribArray
	// This is because vertex attributes are disabled by default
	// The functions takes the location we previously used as its argument
	glEnableVertexAttribArray(0);
	// Colors are designated with location = 1 in the vertex shader
	// The total length of one block is still 8 (3 for vertex positions, 3 for colors and 2 for texture mapping positions)
	// The colors occupy positions 4 through 6 (-> offset 3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture mapping positions are designated with location = 2 in the vertex shader
	// The total length of one block is still 8 (3 for vertex positions, 3 for colors and 2 for texture mapping positions)
	// The texture mapping positions occupy positions 7 through 8 (-> offset 6)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Rectangle::render() {
	//* Bind textures to their corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1ID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2ID);

	//* Do the rendering
	// Tells OpenGL that it is working with this rectangle's VAO
	// Remember that all that OpenGL needs is an VAO; it contains the necessary pointers to both VBOs and EBOs
	glBindVertexArray(VAO);
	// Draw the rectangle; glDrawElements is used here because it uses EBOs stored in the VAO
	// -> used for objects comprised of overlapping vertices designated with an index buffer
	// First argument is the type of object to draw (in thise case still GL_TRIANGLES as our rectangle is comprised of two triangles)
	// Second argument is the number of indices we want to draw, so in this case 6 
	// Third argument is the data type of the indices
	// 4th argument can be used to specify an offset for the EBO, since our data starts at 0 in the EBO, we can leave it at that
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Cube::Cube(const std::string& texture1Path, const std::string& texture2Path) {
	initializeTextures(texture1Path, texture2Path);
	initializeVAO();
}

void Cube::initializeTextures(const std::string& texture1Path, const std::string& texture2Path) {
	loadTexture(texture1Path, texture1ID);
	loadTexture(texture2Path, texture2ID);
}

void Cube::initializeVAO() {
	std::array<float, 180> vertices = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	};

	//* Create a vertex array object that tells OpenGL how to interpret VBO data (see down below)
	// First parameter defines number of VAOs created, seconds takes a pointer to the VAO ID address
	glGenVertexArrays(1, &VAO);

	//* Create a vertex buffer object that stores our data
	// Since we won't change the vertex data, there is no need for us to store the VBO ID permanently
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//* Copy our vertices into a vertex buffer for OpenGL to use
	// This function tells OpenGL which VAO we are currently working with
	glBindVertexArray(VAO);
	// Same for the VBO
	// First parameter tells OpenGL which type of buffer it has to deal with, second parameter is the VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Here the actual data copying happens
	// Static draw specifies that the data is set only once and used many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	///* Tell OpenGL how our vertex data is organised
	// First argument is the location where the set of data is stored in the vertex shaders
	// Second argument is the size of the specified vertex attribute
	// Third argument specifies the data type of the specified vertex attribute
	// 4th argument specifies if we want the data to be normalized to [-1;1] / [0;1], which we don't need, so we use GL_FALSE
	// 5th argument defines the stride, which means the space between consecutive vertex attributes
	// 6th argument defines the offset, which means after which space the specified attribute data is stored
	// Vertex Positions are designated with location = 0 in the vertex shader
	// The total length of one block is 5 (3 for vertex positions and 2 for texture mapping positions)
	// The vertex positions occupy the first 3 positions (-> offset 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// After each specification with glVertexAttribPointer, we need to call glEnableVertexAttribArray
	// This is because vertex attributes are disabled by default
	// The functions takes the location we previously used as its argument
	glEnableVertexAttribArray(0);
	// Texture mapping positions are designated with location = 1 in the vertex shader
	// The total length of one block is still 5 (3 for vertex positions and 2 for texture mapping positions)
	// The texture mapping positions occupy positions 4 and 5 (-> offset 3)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Cube::calculateModelMatrix(Shader& shader, glm::vec3 cubePosition) {
	glm::mat4 translationMatrix = glm::mat4(1.0f); // Start off with an identity matrix
	translationMatrix = glm::translate(translationMatrix, cubePosition); // Translate by the given vector

	// Rotate the cubes at different speeds
	// We'll use quaternions as those save at least 50% computation time over Euler rotations
	float angle = 20.0f * (cubePosition[0] + 1);
	// Note, and this is very important: Quaternions do, in any case, need a normalized vector! Or else they will behave very weirdly
	glm::quat rotationQuaternion = glm::angleAxis((float)glfwGetTime() * glm::radians(angle), glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f)));
	glm::mat4 rotationMatrix = glm::toMat4(rotationQuaternion);
	glm::mat4 modelMatrix = translationMatrix * rotationMatrix;

	// Update the model matrix
	shader.setMat4("modelMatrix", modelMatrix);
}

void Cube::renderMultiple(Shader& shader, std::vector<glm::vec3>& cubePositions) {
	//* Bind textures to their corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1ID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2ID);

	//* Do the rendering
	// Tells OpenGL that it is working with this rectangle's VAO
	// Remember that all that OpenGL needs is an VAO; it contains the necessary pointers to VBOs
	glBindVertexArray(VAO);

	// The previous steps have to be done only once because the VAO and textures used don't change when we re-use the same object multiple times
	for (unsigned int i = 0; i < cubePositions.size(); i++) {
		// Update the model matrix to translate the cube to another position in the world
		calculateModelMatrix(shader, cubePositions[i]);
		
		// Draw the cube
		// First argument is the type of object to draw, in this case triangles as our cube is still comprised of those
		// The second one is the starting index of the array (so in this case 0)
		// The third one is the amount of vertices of the object (a cube has two triangles with 3 vertices each per side, so 2 * 3 * 6 = 36)
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

Plane::Plane() {
	initializeVAO();
}

void Plane::initializeVAO() {
	std::array<float, 20> vertices = {
		// positions; note that we have to use four coordinates x, y, z, w to simulate infinity
		// w = 1.0f means we are calculating a "normal" space coordinate, in this case the middle of the world, 0-0-0
		// w = 0.0f means we are going to infinity
		0.0f, -0.1f, 0.0f, 1.0f,
		1.0f, -0.1f, 1.0f, 0.0f,
		1.0f, -0.1f, -1.0f, 0.0f,
		-1.0f, -0.1f, -1.0f, 0.0f,
		-1.0f, -0.1f, 1.0f, 0.0f,
	};

	std::array<unsigned int, 12> indices = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
	};

	// Create an element buffer object -> manages objects comprised of overlapping vertices
	// Since we won't change the index data, there is no need for us to store the EBO ID permanently
	unsigned int EBO;
	// First parameter defines number of EBOs created, seconds takes a pointer to the EBO ID address
	glGenBuffers(1, &EBO);

	//* Create a vertex array object that tells OpenGL how to interpret VBO data (see down below)
	// First parameter defines number of VAOs created, seconds takes a pointer to the VAO ID address
	glGenVertexArrays(1, &VAO);

	//* Create a vertex buffer object that stores our data
	// Since we won't change the vertex data, there is no need for us to store the VBO ID permanently
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//* Copy our vertices into a vertex buffer for OpenGL to use
	// This function tells OpenGL which VAO we are currently working with
	glBindVertexArray(VAO);
	// Same for the VBO
	// First parameter tells OpenGL which type of buffer it has to deal with, second parameter is the VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Here the actual data copying happens
	// Static draw specifies that the data is set only once and used many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	//* Copy our index array in an element buffer for OpenGL to use
	// Same as for the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	//* Tell OpenGL how our vertex data is organised
	// First argument is the location where the set of data is stored in the vertex shaders
	// Second argument is the size of the specified vertex attribute
	// Third argument specifies the data type of the specified vertex attribute
	// 4th argument specifies if we want the data to be normalized to [-1;1] / [0;1], which we don't need, so we use GL_FALSE
	// 5th argument defines the stride, which means the space between consecutive vertex attributes
	// 6th argument defines the offset, which means after which space the specified attribute data is stored
	// Vertex positions are designated with location = 0 in the vertex shader
	// The total length of one block is 4 (4 vertex positions)
	// The vertex positions occupy the first 4 positions (-> offset 0)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// After each specification with glVertexAttribPointer, we need to call glEnableVertexAttribArray
	// This is because vertex attributes are disabled by default
	// The functions takes the location we previously used as its argument
	glEnableVertexAttribArray(0);
}

void Plane::render() {
	//* Do the rendering
	// Tells OpenGL that it is working with this rectangle's VAO
	// Remember that all that OpenGL needs is an VAO; it contains the necessary pointers to both VBOs and EBOs
	glBindVertexArray(VAO);
	// Draw the rectangle; glDrawElements is used here because it uses EBOs stored in the VAO
	// -> used for objects comprised of overlapping vertices designated with an index buffer
	// First argument is the type of object to draw (in thise case still GL_TRIANGLES as our plane is comprised of four triangles)
	// Second argument is the number of indices we want to draw, so in this case 12
	// Third argument is the data type of the indices
	// 4th argument can be used to specify an offset for the EBO, since our data starts at 0 in the EBO, we can leave it at that
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}

void Render::initialize(GLFWwindow& window) {
	//* Setup OpenGL
	// Enable depth testing (otherwise vertices may override each other); only needed for 3D applications
	glEnable(GL_DEPTH_TEST);
	// Tell OpenGL to capture the mouse
	glfwSetInputMode(&window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Register mouse callback and zoomwheel callback
	glfwSetCursorPosCallback(&window, Input::processMouse);
	glfwSetScrollCallback(&window, Input::processScrollwheel);
	
	//* Setup STBI
	// This is important so that images aren't flipped when they are loaded
	stbi_set_flip_vertically_on_load(true);
}

void Render::clearWindow() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// Clears both the color buffer and the depth buffer so that the values of the previous frame get discarded
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::updateBlendValue(Shader& shader, float delta) {
	blendValue += delta;
	if (blendValue > 1.0f) {
		blendValue = 1.0f;
	}
	else if (blendValue < 0.0f) {
		blendValue = 0.0f;
	}

	shader.setFloat("blendValue", blendValue);
}

void Render::updateMatrices(Shader& shader) {
	// To do changes:
	// update projection matrix only when camera is moved
	// update model matrix only when object is moving
	// update projection matrix only when fov or aspect ratio has changed
	
	Camera cam = ResourceManager::giveCamera();
	// We don't have to change the model matrix as each object handles it on its own

	// For the view matrix, retrieve it from the camera
	glm::mat4 viewMatrix = cam.calculateViewMatrix();

	// For the projection matrix, retrieve the FOV from the camera
	// Third and fourth parameter determine the near plane and far plane distance relative to the camera
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(cam.fov), Window::getAspectRatio(), 0.1f, 100.0f);
	
	shader.setMat4("viewMatrix", viewMatrix);
	shader.setMat4("projectionMatrix", projectionMatrix);
}