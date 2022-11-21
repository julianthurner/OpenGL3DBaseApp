#include <memory>

#include "resourceManager.hpp"
#include "render.hpp"
#include "window.hpp"
#include "input.hpp"

#include <iostream>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

//** Private **//
// Due to C++ immediately defining object declarations (which is very inflexible), we use smart pointers to store camera and floor plane
std::unique_ptr<Camera> cam;
unsigned int uniformBufferID;
std::vector<Shader> shaders;
std::unique_ptr<Plane> plane;
std::vector<Cube> cubes;
std::vector<std::vector<glm::vec3>> objectPositions;
glm::vec3 vector = glm::vec3(1.0f, 0.0f, 0.0f);

void prepareShaders() {
	// Prepare all needed shaders 
	shaders = {
		// [0] => Plane shader, [1] = Cube shader
		Shader("res/shaders/planeShader.vert", "res/shaders/planeShader.frag"),
		Shader("res/shaders/cubeShader.vert", "res/shaders/cubeShader.frag"),
	};

	//* Initialize the uniforms (should be moved to separate function later)
	// Don't forget to activate the shader before setting uniforms
	shaders[1].use();
	// Note that we don't give the stored textureID that OpenGL assigned because those have nothing to do with this
	// This basically tells OpenGL "Use the first assigned texture (0) as uniform texture1, second one (1) as texture2, etc"
	shaders[1].setInt("texture1", 0);
	shaders[1].setInt("texture2", 1);

	// Set initial blend value
	Render::updateBlendValue(shaders[1], 0.5f);

	//* Initialize the uniform buffer object (UBO)
	// First argument is the number of buffers to create, second one is a pointer for storing the ID
	glGenBuffers(1, &uniformBufferID);
	// Tells OpenGL we are currently working with this UBO
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferID);
	// Here the actual data copying happens
	// First argument is the buffer type, second argument is the sice of the UBO (we are storing two 4x4 matrices = 2 * 64 bytes)
	// Third argument is a pointer to the data, but since we don't have any data yet, we give NULL
	// Fourth argument is the data usage pattern: Static draw specifies that the data is set only once and used many times
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	// Link the UBO to binding point 0
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniformBufferID);

	//* Configure the shaders to accept our UBO
	unsigned int shaderUniformIndex;
	for (unsigned int i = 0; i < shaders.size(); i++) {
		// Get the shader's block index for the UBO
		shaderUniformIndex = glGetUniformBlockIndex(shaders[i].shaderProgramID, "matrices");
		// Link the shader's UBO block index to binding point 0 (where we linked the UBO earlier)
		glUniformBlockBinding(shaders[i].shaderProgramID, shaderUniformIndex, 0);
	}
}

void prepareObjects() {
	plane.reset(new Plane());
	
	cubes = {
		Cube("res/images/dummyImage1.jpg", "res/images/dummyImage2.png"),
		Cube("res/images/dummyImage3.png", "res/images/dummyImage4.png"),
		Cube("res/images/dummyImage5.png", "res/images/dummyImage6.png"),
	};

	objectPositions = {
		{ // [0] => Cube positions
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f),
		},
		{
			glm::vec3(3.0f, 2.0f, 0.0f),
		},
		{
			glm::vec3(-2.0f, 0.5f, 0.5f),
		},
	};
}

//** Public **//
void ResourceManager::initialize(GLFWwindow& window) {
	// Initialize key settings
	Render::initialize(window);
	
	// Create a camera
	cam.reset(new Camera(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		2.5f, 150.0f));

	// Prepare shaders and objects
	prepareShaders();
	prepareObjects();

	// Set initial view matrix and projection matrix
	// Note that since the matrices are stored in a uniform buffer object (UBO), this has to be done after shader creation
	// Since the UBO is generated in prepareShaders() and cannot be filled with data if it doesn't exist yet
	cam->updateViewMatrix();
	cam->updateProjectionMatrix();
}

void ResourceManager::render() {
	Render::clearWindow();

	// process plane
	shaders[0].use();
	plane->render();

	// Process cubes
	shaders[1].use(); // Use cube shader. If no other shader is ever used, this line can be moved out of the while loop
	cubes[0].renderMultiple(shaders[1], objectPositions[0]);
	cubes[1].renderMultiple(shaders[1], objectPositions[1]);
	cubes[2].renderMultiple(shaders[1], objectPositions[2]);
}

Camera& ResourceManager::giveCamera() {
	return *cam;
}

std::vector<Shader>& ResourceManager::giveShaders() {
	return shaders;
}

void ResourceManager::setViewMatrix(glm::mat4 viewMatrix) {
	// Tells OpenGL we are currently working with this uniform buffer object (UBO)
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferID);
	// Copy the data into the UBO
	// First argument is the buffer type
	// Second argument is the offset (view matrix occupies bytes 0-63, projection matrix 64-127)
	// Third argument is the data size (= 64 bytes) and 4th argument a pointer to the data
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix);
}

void ResourceManager::setProjectionMatrix(glm::mat4 projectionMatrix) {
	// Tells OpenGL we are currently working with this uniform buffer object (UBO)
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferID);
	// Copy the data into the UBO
	// First argument is the buffer type
	// Second argument is the offset (view matrix occupies bytes 0-63, projection matrix 64-127)
	// Third argument is the data size (= 64 bytes) and 4th argument a pointer to the data
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix);
}