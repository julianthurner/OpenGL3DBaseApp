#include <iostream>

#include "window.hpp"
#include "input.hpp"
#include "testrender.hpp"
#include "shaders.hpp"
#include "camera.hpp"

int main(int argc, char** argv) {
	int resCode;
	GLFWwindow* window = initializeWindow(&resCode);
	if (window == NULL)	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		resCode = -1;
	}

	testfunction5();

	Shader dummyShader = Shader("res/shaders/dummyShader.vert", "res/shaders/dummyShader.frag");

	//Triangle* triangle1 = new Triangle();
	//unsigned int VAO1 = testfunction(triangle1);
	//Triangle* triangle2 = new Triangle();
	//unsigned int VAO2 = testfunction(triangle2);
	//Rectangle* rectangle = new Rectangle("res/images/dummyImage1.jpg", "res/images/dummyImage2.png");
	//Rectangle* rectangle2 = new Rectangle("res/images/dummyImage1.jpg", "res/images/dummyImage2.png");
	//unsigned int VAO1 = testfunction2(rectangle);
	//unsigned int VAO2 = testfunction2(rectangle2);
	Cube* cube = new Cube("res/images/dummyImage1.jpg", "res/images/dummyImage2.png");
	unsigned int VAO = testfunction8(cube);

	Camera* cam = new Camera(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		2.5f);

	dummyShader.use();

	// Initialize the uniforms (should be moved to separate function later)
	// Note that we don't give the stored textureID that OpenGL assigned because those start at 1,
	// whereas the IDs in the fragment shader start at 0
	dummyShader.setInt("texture1", 0);
	dummyShader.setInt("texture2", 1);

	testfunction6(&dummyShader, 0.5f);

	//testfunction4(rectangle);
	//testfunction4(rectangle2);
	testfunction9(cube);

	// Enable depth testint (otherwise vertices may override each other)
	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	// Tell OpenGL to capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Register mouse callback and zoomwheel callback
	glfwSetCursorPosCallback(window, processMouse);
	glfwSetScrollCallback(window, processScrollwheel);

	initializeMouse(cam);

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
		testfunction3();

		testfunction7(&dummyShader, cam);

		//glBindTexture(GL_TEXTURE_2D, rectangle->textureID);

		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//updateColor(shaderProgram);
		//glBindVertexArray(VAO1);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++) {
			testfunction10(&dummyShader, i);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(VAO);


		//testfunction8(&dummyShader);
		//glBindVertexArray(VAO2);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(VAO2);


		
		// Check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwTerminate();
	resCode = 0;

	return resCode;
}

/*
* To dos:
* Remove Testrender dummy
* Remove dummyShaders
* Cleanup main function
* Define key bindings
* Cleanup includes
* Change the disclaimer to better reflect which code is my own
* Add missing documentation
* Optimize the code (-> object only re-calculated when moved or altered in some way; camera only recalculated when moved)
* Use more unsigned ints and more consts
* Optimize readability of the code
* Catch more errors
* Change the functions and classes so that static objects are given only once at initialization
*/