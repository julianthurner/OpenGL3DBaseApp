// Remove later
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "input.hpp"
#include "camera.hpp"
#include "render.hpp"
#include "window.hpp"
#include "resourceManager.hpp"

//** Private **//
bool downKeyPressed = false, upKeyPressed = false;
double mouse_last_x, mouse_last_y;
bool mouse_initialize = true;

//** Public **//
void Input::processKeyboardInput(GLFWwindow& window, float deltaTime) {
	// Fetch camera
	Camera& cam = ResourceManager::giveCamera();
	
	// If user presses Esc, exit the application. Dummy, will be replaced later on
	if (glfwGetKey(&window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(&window, true);
	}
	if (glfwGetKey(&window, GLFW_KEY_1) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(&window, GLFW_KEY_2) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(&window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (!upKeyPressed) {
			Render::updateBlendValue(ResourceManager::giveShaders()[1], 0.1f);
			upKeyPressed = true;
		}
	}
	if (glfwGetKey(&window, GLFW_KEY_UP) == GLFW_RELEASE) {
		upKeyPressed = false;
	}

	if (glfwGetKey(&window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (!downKeyPressed) {
			Render::updateBlendValue(ResourceManager::giveShaders()[1], -0.1f);
			downKeyPressed = true;
		}
	}
	if (glfwGetKey(&window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
		downKeyPressed = false;
	}

	if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.cameraPosition += cam.cameraDirectionVector * cam.cameraSpeed * deltaTime;
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.cameraPosition -= cam.cameraDirectionVector * cam.cameraSpeed * deltaTime;
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.cameraPosition -= cam.cameraRightVector * cam.cameraSpeed * deltaTime;
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.cameraPosition += cam.cameraRightVector * cam.cameraSpeed * deltaTime;
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_Q) == GLFW_PRESS) {
		cam.updateRotation(-cam.rollSpeed * deltaTime); // Positive angles rotate clockwise
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_E) == GLFW_PRESS) {
		cam.updateRotation(cam.rollSpeed * deltaTime); // Negative angles rotate counter-clockwise
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cam.cameraPosition += cam.cameraUpVector * cam.cameraSpeed * deltaTime;
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cam.cameraPosition -= cam.cameraUpVector * cam.cameraSpeed * deltaTime;
		cam.updateViewMatrix();
	}
}

// Callback function
// x and y hold the mouse's current position
void Input::processMouse(GLFWwindow* window, double current_x, double current_y) {
	// Fetch camera
	Camera& cam = ResourceManager::giveCamera();
	
	if (mouse_initialize) {
		mouse_last_x = current_x;
		mouse_last_y = current_y;
		mouse_initialize = false;
	}

	float mouseSensitivity = cam.mouseSensitivity;

	double xOffset = mouse_last_x - current_x; // Reversed due to rotation being clockwise for positive values and vise versa
	double yOffset = mouse_last_y - current_y; // Same here
	mouse_last_x = current_x;
	mouse_last_y = current_y;

	cam.updateYawAndPitch((float)(xOffset * mouseSensitivity), (float)(yOffset * mouseSensitivity));
}

// Callback function
// xOffset and yOffset hold the scroll wheel's offset
void Input::processScrollwheel(GLFWwindow* window, double xOffset, double yOffset) {
	// Fetch camera
	Camera& cam = ResourceManager::giveCamera();
	
	float fov = cam.fov;
	// Subtract because scrolling forward increases the yOffset, but zooming inwards means a lower FOV value
	fov -= (float)yOffset;
	// Set limits to the zoom level so that we don't get any weird flips at 0° and 90°
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	else if (fov > 89.0f) {
		fov = 89.0f;
	}
	cam.fov = fov;

	// Since the projection matrix has changed, update it
	cam.updateProjectionMatrix();
}