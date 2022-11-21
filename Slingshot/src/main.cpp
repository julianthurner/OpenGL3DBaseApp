#include <iostream>
#include <string>

#include "window.hpp"
#include "input.hpp"
#include "render.hpp"
#include "shaders.hpp"
#include "camera.hpp"
#include "resourceManager.hpp"

int main(int argc, char** argv) {
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

	glfwTerminate();

	return 0;
}

/*
* To dos:
* Use vectors wherever possible
* Make sure there are as few raw pointers as possible
* Use more unsigned ints and more consts
* Check if const-ness is correct in class files
* Use std:endl for outputs
* Catch more errors
* Optimize documentation, esp. for OpenGL functions (+ should function documentation be moved into header files?)
* 
* Cleanup includes (glad and glfw are different)
* Change the disclaimer to better reflect which code is my own
* 
* Replace dummy pictures
* 
* Is there any way to publich the includes?
*/