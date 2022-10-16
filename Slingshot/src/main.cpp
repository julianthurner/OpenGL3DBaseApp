#include <iostream>

#include "window.hpp"
#include "input.hpp"
#include "testrender.hpp"
#include "shaders.hpp"

int main(int argc, char** argv) {
	int resCode;
	GLFWwindow* window = initializeWindow(&resCode);
	if (window == NULL)	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		resCode = -1;
	}

	Shader dummyShader = Shader("res/shaders/dummyShader.vert", "res/shaders/dummyShader.frag");

	Triangle* triangle1 = new Triangle();
	unsigned int VAO1 = testfunction(triangle1);
	//Triangle* triangle2 = new Triangle();
	//unsigned int VAO2 = testfunction(triangle2);
	//Rectangle* rectangle = new Rectangle();
	//unsigned int VAO = testfunction2(rectangle);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Process the user's key presses
		processInput(window);

		// Render
		testfunction3();
		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);

		dummyShader.use();
		//updateColor(shaderProgram);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(VAO2);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
		
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
*/