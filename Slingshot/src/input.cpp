#include "input.hpp"
#include "testrender.hpp"

bool downKeyPressed = false;
bool upKeyPressed = false;

void processInput(GLFWwindow* window, Shader* shader) {
	// If user presses Esc, exit the application. Dummy, will be replaced later on
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (!upKeyPressed) {
			testfunction6(shader, 0.1f);
			upKeyPressed = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
		upKeyPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (!downKeyPressed) {
			testfunction6(shader, -0.1f);
			downKeyPressed = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
		downKeyPressed = false;
	}
}