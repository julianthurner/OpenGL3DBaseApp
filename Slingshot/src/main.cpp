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

	testfunction5();

	Shader dummyShader = Shader("res/shaders/dummyShader.vert", "res/shaders/dummyShader.frag");

	//Triangle* triangle1 = new Triangle();
	//unsigned int VAO1 = testfunction(triangle1);
	//Triangle* triangle2 = new Triangle();
	//unsigned int VAO2 = testfunction(triangle2);
	Rectangle* rectangle = new Rectangle("res/images/dummyImage1.jpg", "res/images/dummyImage2.png");
	Rectangle* rectangle2 = new Rectangle("res/images/dummyImage1.jpg", "res/images/dummyImage2.png");
	unsigned int VAO1 = testfunction2(rectangle);
	unsigned int VAO2 = testfunction2(rectangle2);

	dummyShader.use();

	// Initialize the uniforms (should be moved to separate function later)
	// Note that we don't give the stored textureID that OpenGL assigned because those start at 1,
	// whereas the IDs in the fragment shader start at 0
	dummyShader.setInt("texture1", 0);
	dummyShader.setInt("texture2", 1);

	testfunction6(&dummyShader, 0.5f);

	testfunction4(rectangle);
	testfunction4(rectangle2);

		// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Process the user's key presses
		processInput(window, &dummyShader);

		// Render
		testfunction3();

		testfunction7(&dummyShader);

		//glBindTexture(GL_TEXTURE_2D, rectangle->textureID);

		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO1);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//updateColor(shaderProgram);
		//glBindVertexArray(VAO1);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(VAO2);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAO1);


		testfunction8(&dummyShader);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAO2);
		
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
* Check if const-ness is correct in class files
*/