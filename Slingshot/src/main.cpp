#include <iostream>

#include "window.hpp"
#include "input.hpp"
#include "render.hpp"
#include "shaders.hpp"
#include "camera.hpp"

int main(int argc, char** argv) {
	int resCode;
	GLFWwindow* window = initializeWindow(&resCode);

	initializeRender();

	Shader dummyShader = Shader("res/shaders/dummyShader.vert", "res/shaders/dummyShader.frag");
	
	Cube cube = Cube("res/images/dummyImage1.jpg", "res/images/dummyImage2.png");

	Camera cam = Camera(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		2.5f, 150.0f);

	dummyShader.use();

	// Initialize the uniforms (should be moved to separate function later)
	// Note that we don't give the stored textureID that OpenGL assigned because those start at 1,
	// whereas the IDs in the fragment shader start at 0
	dummyShader.setInt("texture1", 0);
	dummyShader.setInt("texture2", 1);

	// Test what happens if you load multiple objects with different textures

	updateBlendValue(&dummyShader, 0.5f);

	// Enable depth testing (otherwise vertices may override each other)
	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	// Tell OpenGL to capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Register mouse callback and zoomwheel callback
	glfwSetCursorPosCallback(window, processMouse);
	glfwSetScrollCallback(window, processScrollwheel);

	initializeInput(&cam);

	glm::vec3* cubePositions = new glm::vec3[10] {
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
	};

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate frame times
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// Process the user's key presses
		processKeyboardInput(window, &dummyShader, deltaTime);

		// Render
		clearWindow();
		updateMatrices(&dummyShader, &cam);
		cube.renderMultiple(&dummyShader, cubePositions);

		// Check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	delete[] cubePositions;
	glfwTerminate();
	resCode = 0;

	return resCode;
}

/*
* To dos:
* Find out if we can fetch RGB data from the image
* Implement a resource manager
* Write a function initializeShaders that sets up all the needed shaders with textures and stuff
* Cleanup main function
* Cleanup includes
* Make sure that every "new" has a "delete"
* Change the disclaimer to better reflect which code is my own
* Add missing documentation
* Optimize the code (-> object only re-calculated when moved or altered in some way; camera only recalculated when moved)
* Use more unsigned ints and more consts
* Check if const-ness is correct in class files
* Optimize readability of the code
* Catch more errors
* Change the functions and classes so that static objects are given only once at initialization
* Use references instead of pointers where applicable
* Should function documentation be moved into header files?
* Replace dummy pictures
* Make the shaders generic and rename them
* Implement a floor plane
* Use std:endl for outputs
* Implement quaternions
*/