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
	GLFWwindow& window = initializeWindow();

	initializeResourceManager(window);
	
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	// Main loop
	while (!glfwWindowShouldClose(&window))
	{
		// Calculate frame times
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// Process the user's key presses
		processKeyboardInput(&window, deltaTime);
		// glfwPollEvents calls the callbacks set in initializeRender() => processMouse() and processScrollwheel()
		glfwPollEvents();

		// Render
		render();
		
		// Check and call events and swap buffers
		glfwSwapBuffers(&window);
	}

	glfwTerminate();

	return 0;
}

/*
* To dos:
* Find a better solution for the default camera constructor problem
* Cleanup render class and shader class
* Find out if how texture IDs behave when assigning different textures to different shaders
* Use vectors wherever possible
* Implement a resource manager
* Turn everything into classes / statics
* Write a function initializeShaders that sets up all the needed shaders with textures and stuff
* Cleanup main function
* Cleanup includes (glad and glfw are different)
* Make sure that every "new" has a "delete" or better yet make sure no raw pointers exist at all
* Replace pointers with objects where possible
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
* Implement a floor plane
* Use std:endl for outputs
* Implement quaternions
*/