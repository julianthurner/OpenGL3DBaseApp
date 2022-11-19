#include <memory>

#include "resourceManager.hpp"
#include "render.hpp"
#include "window.hpp"
#include "input.hpp"

#include <iostream>

//** Private **//
// Due to C++ immediately defining object declarations (which is very inflexible), we use smart pointers to store camera and floor plane
std::unique_ptr<Camera> cam;
std::vector<Shader> shaders;
std::unique_ptr<Plane> plane;
std::vector<Cube> cubes;
std::vector<std::vector<glm::vec3>> objectPositions;

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
}

void ResourceManager::render() {
	Render::clearWindow();

	// process plane
	shaders[0].use();
	Render::updateMatrices(shaders[0]);
	plane->render();

	// Process cubes
	shaders[1].use(); // Use cube shader. If no other shader is ever used, this line can be moved out of the while loop
	Render::updateMatrices(shaders[1]);
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