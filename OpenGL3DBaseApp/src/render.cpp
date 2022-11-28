#include <iostream>

#include <glm/gtx/quaternion.hpp>

// Seems strange, but this is the correct way of including stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

#include "render.hpp"
#include "input.hpp"

//** Private **//
float blendValue;

void loadTexture(const std::string& texturePath, unsigned int& textureID) {
	int width, height, numberOfColorChannels;
	unsigned int rgbType = 0;
	//* Load the image from file
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

		if (numberOfColorChannels < 0) {
			std::cout << "Are you sure the specified file exists?\n" << std::endl;
		}
	}

	//* Generate a new empty texture. First argument is the number of textures to create
	// Second argument is a pointer to store the assigned ID in
	glGenTextures(1, &textureID);
	// Tells OpenGL which texture we are currently working with. First argument specifies the texture type, second one takes the ID
	glBindTexture(GL_TEXTURE_2D, textureID);

	//* Set texture wrapping to "Mirror" on both axes (-> S-axe and T-axe which correspond to x and y)
	// First argument is texture type, second argument the parameter to modify, third argument the desired value of the parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Set texture filtering to "Linear"
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set mipmapping to "Linear / Linear"
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//* Fill the empty texture with the previously loaded image data
	if (imageData) {
		// First argument is the texture type, second one is for setting mipmap levels manually which we don't want to do -> leave at 0
		// Third argument tells OpenGL the format the texture is to be stored in, 4th and 5th argument should be self-explanatory
		// 6th argument has to be 0 at all times (specifies border width; legacy)
		// 7th argument specifies the format of the source image
		// 8th argument specifies the datatype of the source image; since we stored it as unsigned char (-> byte), this has to be GL_UNSIGNED_BYTE
		// 9th argument is a pointer to the actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, rgbType, width, height, 0, rgbType, GL_UNSIGNED_BYTE, imageData);
		// This function automatically creates mipmaps for the current texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Error: Failed to load texture\n" << std::endl;
	}

	// Free the image from memory
	stbi_image_free(imageData);
}

void create_VAO_VBO(const std::vector<float>& vertices, unsigned int& VAO_ID) {
	//* Create a vertex array object (VAO) that tells OpenGL how to interpret vertex buffer array (VBO) data (see down below)
	// First argument defines number of VAOs created, seconds takes a pointer to store the assigned VAO ID in
	glGenVertexArrays(1, &VAO_ID);

	//* Create a VBO that stores our data
	// Since we won't change the vertex data, there is no need for us to store the VBO ID permanently
	unsigned int VBO_ID;
	// First argument defines number of VBOs created, seconds takes a pointer to store the assigned VBO ID in
	glGenBuffers(1, &VBO_ID);

	//* Copy our data into the VBO
	// This function tells OpenGL which VAO we are currently working with. First argument is the VAO ID
	glBindVertexArray(VAO_ID);
	// Same for the VBO
	// First argument tells OpenGL which type of buffer it has to deal with, second argument is the VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	// Here the actual data copying happens
	// First argument is the buffer type, second argument the size of the data, third argument a pointer to the data
	// Note that since we are passing a std::vector, we need to give a pointer to the first item
	// 4th argument tells OpenGL how to handle the data. "Static draw" specifies that the data is set only once and used many times
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
}

void create_VAO_VBO_EBO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int& VAO_ID) {
	//* Create an element buffer object (EBO) -> manages objects comprised of overlapping vertices that are specified with indices
	// Since we won't change the index data, there is no need for us to store the EBO ID permanently
	unsigned int EBO_ID;
	// First argument defines number of EBOs created, seconds takes a pointer to store the assigned EBO ID in
	glGenBuffers(1, &EBO_ID);

	//* Create a vertex array object (VAO) that tells OpenGL how to interpret vertex buffer array (VBO) data (see down below)
	// First argument defines number of VAOs created, seconds takes a pointer to store the assigned VAO ID in
	glGenVertexArrays(1, &VAO_ID);

	//* Create a VBO that stores our data
	// Since we won't change the vertex data, there is no need for us to store the VBO ID permanently
	unsigned int VBO_ID;
	// First argument defines number of VBOs created, seconds takes a pointer to store the assigned VBO ID in
	glGenBuffers(1, &VBO_ID);

	//* Copy our data into the VBO
	// This function tells OpenGL which VAO we are currently working with. First argument is the VAO ID
	glBindVertexArray(VAO_ID);
	// Same for the VBO
	// First argument tells OpenGL which type of buffer it has to deal with, second argument is the VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	// Here the actual data copying happens
	// First argument is the buffer type, second argument the size of the data, third argument a pointer to the data
	// Note that since we are passing a std::vector, we need to give a pointer to the first item
	// 4th argument tells OpenGL how to handle the data. "Static draw" specifies that the data is set only once and used many times
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//* Copy our indices into an element buffer for OpenGL to use
	// Same as for the VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);
}

//** Public **//
Triangle::Triangle() {
	std::vector<float> vertices = {
		// positions			// colors
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f    // top 
	};
	initializeVAO(vertices);
}

Triangle::Triangle(const std::vector<float>& vertices) {
	initializeVAO(vertices);
}

void Triangle::initializeVAO(const std::vector<float>& vertices) {
	create_VAO_VBO(vertices, VAO_ID);

	//* Tell OpenGL how our vertex data is organised
	// First argument is the location where the specified set of data is stored in the vertex shaders
	// Second argument is the size of the specified set of data
	// Third argument is the data type of the specified set of data
	// 4th argument specifies if we want the data to be normalized to [-1;1] / [0;1], which we don't need, so we use GL_FALSE
	// 5th argument defines the stride, which means the space between consecutive data blocks
	// 6th argument defines the offset, which means after which space in a block the specified set of data starts
	// The 6th argument has to be cast to (void*)
	// Vertex positions are designated with location = 0 in the vertex shader and are comprised of 3 values
	// The total length of one block is 6 (3 for positions and 3 for colors)
	// The vertex positions occupy the first 3 positions (-> offset 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// After each specification with glVertexAttribPointer, we need to call glEnableVertexAttribArray
	// This is because vertex attributes are disabled by default
	// The functions takes the location we previously used as its argument
	glEnableVertexAttribArray(0);
	// Colors are designated with location = 1 in the vertex shader and are comprised of 3 values
	// The total length of one block is still 6 (3 for positions and 3 for colors)
	// The colors occupy the second 3 positions (-> offset 3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Triangle::render() {
	//* Do the rendering
	// Tells OpenGL that it is working with this triangle's VAO
	// Remember that all that OpenGL needs is an VAO; it contains the necessary pointers to VBOs
	glBindVertexArray(VAO_ID);
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
	std::vector<float> vertices = {
		// positions          // colors           // texture coordinates
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	std::vector<unsigned int> indices = {
		0, 1, 3,	// first triangle
		1, 2, 3,	// second triangle
	};

	create_VAO_VBO_EBO(vertices, indices, VAO_ID);

	//* Tell OpenGL how our vertex data is organised
	// First argument is the location where the specified set of data is stored in the vertex shaders
	// Second argument is the size of the specified set of data
	// Third argument is the data type of the specified set of data
	// 4th argument specifies if we want the data to be normalized to [-1;1] / [0;1], which we don't need, so we use GL_FALSE
	// 5th argument defines the stride, which means the space between consecutive data blocks
	// 6th argument defines the offset, which means after which space in a block the specified set of data starts
	// The 6th argument has to be cast to (void*)
	// Vertex positions are designated with location = 0 in the vertex shader and are comprised of 3 values
	// The total length of one block is 8 (3 for vertex positions, 3 for colors and 2 for texture mapping positions)
	// The vertex positions occupy the first 3 positions (-> offset 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// After each specification with glVertexAttribPointer, we need to call glEnableVertexAttribArray
	// This is because vertex attributes are disabled by default
	// The functions takes the location we previously used as its argument
	glEnableVertexAttribArray(0);
	// Colors are designated with location = 1 in the vertex shader and are comprised of 3 values
	// The total length of one block is still 8 (3 for vertex positions, 3 for colors and 2 for texture mapping positions)
	// The colors occupy positions 4 through 6 (-> offset 3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture mapping positions are designated with location = 2 in the vertex shader and are comprised of 2 values
	// The total length of one block is still 8 (3 for vertex positions, 3 for colors and 2 for texture mapping positions)
	// The texture mapping positions occupy positions 7 through 8 (-> offset 6)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Rectangle::render() {
	//* Bind textures to their corresponding texture units
	// Tells OpenGL which texture slot to use (there is a max of 16 texture slots to be used at once, GL_TEXTURE0 through GL_TEXTURE15)
	glActiveTexture(GL_TEXTURE0);
	// Tells OpenGL which texture to fill into the slot
	// First argument is the texture type, second argument the texture's assigned ID
	glBindTexture(GL_TEXTURE_2D, texture1ID);
	// Same for the second texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2ID);

	//* Do the rendering
	// Tells OpenGL that it is working with this rectangle's VAO
	// Remember that all that OpenGL needs is an VAO; it contains the necessary pointers to both VBOs and EBOs
	glBindVertexArray(VAO_ID);
	// Draw the rectangle; glDrawElements is used here because it uses EBOs stored in the VAO
	// -> used for objects comprised of overlapping vertices that are specified with indices
	// First argument is the type of object to draw (in thise case still GL_TRIANGLES as our rectangle is comprised of two triangles)
	// Second argument is the number of indices we want to draw, so in this case 6 
	// Third argument is the data type of the indices
	// 4th argument can be used to specify an offset for the EBO; since our data starts at 0 in the EBO, we can leave it at that
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
	std::vector<float> vertices = {
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

	create_VAO_VBO(vertices, VAO_ID);

	//* Tell OpenGL how our vertex data is organised
	// First argument is the location where the specified set of data is stored in the vertex shaders
	// Second argument is the size of the specified set of data
	// Third argument is the data type of the specified set of data
	// 4th argument specifies if we want the data to be normalized to [-1;1] / [0;1], which we don't need, so we use GL_FALSE
	// 5th argument defines the stride, which means the space between consecutive data blocks
	// 6th argument defines the offset, which means after which space in a block the specified set of data starts
	// The 6th argument has to be cast to (void*)
	// Vertex Positions are designated with location = 0 in the vertex shader and are comprised of 3 values
	// The total length of one block is 5 (3 for vertex positions and 2 for texture mapping positions)
	// The vertex positions occupy the first 3 positions (-> offset 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// After each specification with glVertexAttribPointer, we need to call glEnableVertexAttribArray
	// This is because vertex attributes are disabled by default
	// The functions takes the location we previously used as its argument
	glEnableVertexAttribArray(0);
	// Texture mapping positions are designated with location = 1 in the vertex shader and are comprised of 2 values
	// The total length of one block is still 5 (3 for vertex positions and 2 for texture mapping positions)
	// The texture mapping positions occupy positions 4 and 5 (-> offset 3)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Cube::calculateModelMatrix(const Shader& shader, const glm::vec3& cubePosition) {
	glm::mat4 translationMatrix = glm::mat4(1.0f); // Start off with an identity matrix
	translationMatrix = glm::translate(translationMatrix, cubePosition); // Translate by the given vector

	//* Rotate the cubes at different speeds
	// We'll use quaternions for rotations as they save at least 50% computation time over Euler rotations
	// Note, and this is very important: Quaternions do, in any case, need a normalized vector! Or else they will behave very weirdly
	// Make the angle differ between cubes by using the given translation vector's x value for calculating
	float angle = 20.0f * (cubePosition[0] + 1);
	// glm::angleAxis creates a quaternion that stores a rotation
	// First argument is the angle by which to rotate and second argument is the (normalized!) vector to be rotated around
	// Since the given vector is no unit vector, it has to be normalized
	glm::quat rotationQuaternion = glm::angleAxis((float)glfwGetTime() * glm::radians(angle), glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f)));
	// We calculate a rotation matrix from the quaternion
	glm::mat4 rotationMatrix = glm::toMat4(rotationQuaternion);
	glm::mat4 modelMatrix = translationMatrix * rotationMatrix;

	// Update the model matrix
	shader.setMat4("modelMatrix", modelMatrix);

	// Theoretically, it would be possible to store the model matrices for each object locally and only recalculate it if the object is altered in any way
	// However, in a typical video game scene (which has millions of vertices), this would consume tremendous amounts of RAM
	// compared to storing only the object's coordinates
}

void Cube::renderMultiple(const Shader& shader, const std::vector<glm::vec3>& cubePositions) {
	//* Bind textures to their corresponding texture units
	// Tells OpenGL which texture slot to use (there is a max of 16 texture slots to be used at once, GL_TEXTURE0 through GL_TEXTURE15)
	glActiveTexture(GL_TEXTURE0);
	// Tells OpenGL which texture to fill into the slot
	// First argument is the texture type, second argument the texture's assigned ID
	glBindTexture(GL_TEXTURE_2D, texture1ID);
	// Same for the second texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2ID);

	//* Prepare OpenGL for rendering our cube object
	// Tells OpenGL that it is working with this rectangle's VAO
	// Remember that all that OpenGL needs is an VAO; it contains the necessary pointers to VBOs
	glBindVertexArray(VAO_ID);

	//* Do the rendering 
	// The previous steps had to be done only once because the VAO and textures used don't change when we re-use the same object multiple times
	// No we use the same base cube object to render multiple different cubes scattered throughout the scene
	for (unsigned int i = 0; i < cubePositions.size(); i++) {
		//* Update the model matrix to translate the cube to another position in the world and rotate it
		calculateModelMatrix(shader, cubePositions[i]);
		
		//* Draw the cube
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
	std::vector<float> vertices = {
		// positions; note that we have to use four coordinates x, y, z, w to simulate infinity
		// w = 1.0f means we are calculating a "normal" space coordinate, in this case the middle of the scene (0-0-0)
		// w = 0.0f means we are going to infinity
		// Setting y = -0.1f means that the plane is below us at the beginning, otherwise it would be at eye level and thus invisible
		0.0f, -0.1f, 0.0f, 1.0f,	// Middle of the scene
		1.0f, -0.1f, 1.0f, 0.0f,
		1.0f, -0.1f, -1.0f, 0.0f,
		-1.0f, -0.1f, -1.0f, 0.0f,
		-1.0f, -0.1f, 1.0f, 0.0f,
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
	};

	create_VAO_VBO_EBO(vertices, indices, VAO_ID);

	//* Tell OpenGL how our vertex data is organised
	// First argument is the location where the specified set of data is stored in the vertex shaders
	// Second argument is the size of the specified set of data
	// Third argument is the data type of the specified set of data
	// 4th argument specifies if we want the data to be normalized to [-1;1] / [0;1], which we don't need, so we use GL_FALSE
	// 5th argument defines the stride, which means the space between consecutive data blocks
	// 6th argument defines the offset, which means after which space in a block the specified set of data starts
	// The 6th argument has to be cast to (void*)
	// Vertex positions are designated with location = 0 in the vertex shader and are comprised of 4 values
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
	glBindVertexArray(VAO_ID);
	// Draw the rectangle; glDrawElements is used here because it uses EBOs stored in the VAO
	// -> used for objects comprised of overlapping vertices that are specified with indices
	// First argument is the type of object to draw (in thise case still GL_TRIANGLES as our rectangle is comprised of two triangles)
	// Second argument is the number of indices we want to draw, so in this case 6 
	// Third argument is the data type of the indices
	// 4th argument can be used to specify an offset for the EBO; since our data starts at 0 in the EBO, we can leave it at that
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}

void Render::initialize(GLFWwindow& window) {
	//* Setup OpenGL
	// Enable depth testing (otherwise vertices may override each other); only needed for 3D applications
	glEnable(GL_DEPTH_TEST);
	// This specifies the color that the color buffer uses after clearing it with glClear() in Render::clearWindow()
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//* Setup GLFW
	// Tell OpenGL to capture the mouse
	// First argument takes a pointer to the window, second argument is the parameter to modify, third argument is the desired value of the parameter
	glfwSetInputMode(&window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Register mouse callback and zoomwheel callback
	// First argument each is a pointer to the window, second argument the function to be called on callback
	glfwSetCursorPosCallback(&window, Input::processMouse);
	glfwSetScrollCallback(&window, Input::processScrollwheel);
	
	//* Setup STBI
	// This makes sure images are flipped on loading which is important because otherwise they would be flipped on their heads
	stbi_set_flip_vertically_on_load(true);
}

void Render::clearWindow() {
	// Clears both the color buffer and the depth buffer so that the values of the previous frame get discarded
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::updateBlendValue(const Shader& shader, const float delta) {
	blendValue += delta;
	// Blend values <0 or >1 don't make much sense, so prevent them from happening
	if (blendValue > 1.0f) {
		blendValue = 1.0f;
	}
	else if (blendValue < 0.0f) {
		blendValue = 0.0f;
	}

	//* Send the updated value to the shader
	// Don't forget to activate a shader before setting uniforms
	shader.use();
	shader.setFloat("blendValue", blendValue);
}