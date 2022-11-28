// GLAD manages function pointers for OpenGL while GLFW gives us the necessities required for rendering OpenGL to the screen like window, input etc.
// GLAD loads the OpenGL version specified while building it while GLFW relies on the OS minimum OpenGL version which is usually a pretty bad idea
// Thus, always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.hpp"
#include "resourceManager.hpp"
#include "input.hpp"

int main(void) {
	// The window is the only thing we initialize within the main function as we need to access it from here
	GLFWwindow& window = Window::initialize();

	ResourceManager::initialize(window);
	
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	// Main loop
	while (!glfwWindowShouldClose(&window))
	{
		// Calculate frame times
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// Process the user's key presses
		Input::processKeyboardInput(window, deltaTime);
		// glfwPollEvents calls the callbacks set in Render::initialize() => Input::processMouse() and Input::processScrollwheel()
		glfwPollEvents();

		// Render
		ResourceManager::render();
		
		// Swap buffers
		glfwSwapBuffers(&window);
	}
	// Close everything. Will also free all allocated memory.
	glfwTerminate();

	return 0;
}