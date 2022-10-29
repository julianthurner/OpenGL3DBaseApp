#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "input.hpp"
#include "testrender.hpp"
#include "window.hpp"

bool downKeyPressed = false, upKeyPressed = false;
float mouse_last_x, mouse_last_y;
float mouseSensitivity;
bool mouse_initialize = true;
float yaw, pitch;
Camera* cam;

void initializeMouse(Camera* camera) {
	mouseSensitivity = 0.1f;
	yaw = -90.0f; pitch = 0.0f;

	cam = camera;
}

void processKeyboardInput(GLFWwindow* window, Shader* shader, float deltaTime) {
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam->cameraPosition += cam->cameraDirectionVector * cam->cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam->cameraPosition -= cam->cameraDirectionVector * cam->cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::vec3 delta = cam->cameraRightVector * cam->cameraSpeed * deltaTime;
		cam->cameraPosition -= delta;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::vec3 delta = cam->cameraRightVector * cam->cameraSpeed * deltaTime;
		cam->cameraPosition += delta;
	}
}

// x and y hold the mouse's current position
void processMouse(GLFWwindow* window, double current_x, double current_y) {
	if (mouse_initialize) {
		mouse_last_x = current_x;
		mouse_last_y = current_y;
		mouse_initialize = false;
	}
	
	float xOffset = current_x - mouse_last_x;
	float yOffset = mouse_last_y - current_y; // Reversed since y-coordinates range from bottom to top
	mouse_last_x = current_x;
	mouse_last_y = current_y;

	yaw += xOffset * mouseSensitivity;
	pitch += yOffset * mouseSensitivity;

	// Prevents the user from looking further than at the sky or his feet (otherwise we'd get a flip at +/- 90°)
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cam->cameraDirectionVector = glm::normalize(direction);

	// We need to update the right vector specifically (rotation currently isn't possible so the up vector stays the same)
	cam->updateCameraVectors();
}

void processScrollwheel(GLFWwindow* window, double xOffset, double yOffset) {
	// Subtract because scrolling forward increases the yOffset, but zooming inwards means a lower FOV value
	cam->fov -= (float)yOffset;
	// Set limits to the zoom level so that we don't get any weird flips at 0° and 90°
	if (cam->fov < 1.0f) {
		cam->fov = 1.0f;
	}
	else if (cam->fov > 89.0f) {
		cam->fov = 89.0f;
	}	
}