#include <memory>

#include "resourceManager.hpp"
#include "render.hpp"
#include "window.hpp"
#include "input.hpp"

//** Private **//
// Due to C++ immediately defining object declarations (which is very inflexible), we use a smart pointer to store the camera
std::unique_ptr<Camera> cam;
std::vector<Shader> shaders;
std::vector<Cube> cubes;
std::vector<std::vector<glm::vec3>> objectPositions;

void prepareShaders() {
	// Prepare all needed shaders 
	shaders = {
		// [0] => Cube shader
		Shader("res/shaders/cubeShader.vert", "res/shaders/cubeShader.frag"),
	};

	// Don't forget to activate the shader before setting uniforms
	shaders[0].use();

	// Initialize the uniforms (should be moved to separate function later)
	// Note that we don't give the stored textureID that OpenGL assigned because those start at 1,
	// whereas the IDs in the fragment shader start at 0
	// Test what happens if you load multiple objects with different textures
	shaders[0].setInt("texture1", 0);
	shaders[0].setInt("texture2", 1);

	// Set initial blend value
	Render::updateBlendValue(shaders[0], 0.5f);
}

void prepareObjects() {
	cubes = {
		Cube("res/images/dummyImage1.jpg", "res/images/dummyImage2.png"),
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
		}
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
	
	// Process cubes
	shaders[0].use(); // Use cube shader. If no other shader is ever used, this line can be moved out of the while loop
	cubes[0].renderMultiple(shaders[0], objectPositions[0]);
	Render::updateMatrices(shaders[0]);
}

Camera& ResourceManager::giveCamera() {
	return *cam;
}

std::vector<Shader>& ResourceManager::giveShaders() {
	return shaders;
}