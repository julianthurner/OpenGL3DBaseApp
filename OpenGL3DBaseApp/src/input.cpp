#include "input.hpp"
#include "camera.hpp"
#include "resourceManager.hpp"
#include "render.hpp"

//** Private **//
bool downKeyPressed = false, upKeyPressed = false;
double mouse_last_x, mouse_last_y;
bool mouse_initialize = true;

//** Public **//
void Input::processKeyboardInput(GLFWwindow& window, const float deltaTime) {
	// Fetch camera
	Camera& cam = ResourceManager::giveCamera();
	
	// If user presses Esc, exit the application
	if (glfwGetKey(&window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(&window, true);
	}
	// If user presses "1", display only vertex lines
	if (glfwGetKey(&window, GLFW_KEY_1) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	// If user presses "2", display filled vertices
	if (glfwGetKey(&window, GLFW_KEY_2) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// If user presses up / down arrow, increase / decrease the cube shader's blend value
	// The variables upKeyPressed / downKeyPressed are needed to avoid increasing / decreasing the value each frame until the key is released
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

	// Using WASD, the user can move around horizontally
	if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.cameraPosition += cam.cameraDirectionVector * cam.moveSpeed * deltaTime;
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.cameraPosition -= cam.cameraDirectionVector * cam.moveSpeed * deltaTime;
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.cameraPosition -= cam.cameraRightVector * cam.moveSpeed * deltaTime;
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.cameraPosition += cam.cameraRightVector * cam.moveSpeed * deltaTime;
		cam.updateViewMatrix();
	}

	// Using Q and E, the user can "do a barrel roll"  in either direction ;)
	if (glfwGetKey(&window, GLFW_KEY_Q) == GLFW_PRESS) {
		cam.updateRotation(-cam.rollSpeed * deltaTime); // Negative angles rotate counter-clockwise
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_E) == GLFW_PRESS) {
		cam.updateRotation(cam.rollSpeed * deltaTime); // Positive angles rotate clockwise
		cam.updateViewMatrix();
	}

	// Using Space and Shift, the user can move around vertically
	if (glfwGetKey(&window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cam.cameraPosition += cam.cameraUpVector * cam.moveSpeed * deltaTime;
		cam.updateViewMatrix();
	}
	if (glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cam.cameraPosition -= cam.cameraUpVector * cam.moveSpeed * deltaTime;
		cam.updateViewMatrix();
	}
}

// Callback function
// x and y hold the mouse's current position
void Input::processMouse(GLFWwindow* window, double current_x, double current_y) {
	// Fetch camera
	Camera& cam = ResourceManager::giveCamera();
	
	// On first calling, copy the current values
	if (mouse_initialize) {
		mouse_last_x = current_x;
		mouse_last_y = current_y;
		mouse_initialize = false;
	}
	else {
		float mouseSensitivity = cam.mouseSensitivity;

		// Rotations are clockwise for positive values and counter-clockwise for negative values
		// If the x value increases (= mouse goes to the right), our yawOffset has to be negative
		// -> Because we are rotating around the cameraUpVector, we need a counter-clockwise rotation for the camera to move to the right
		// Thus, we subtract the current x from the last x so that an increase in x gives a negative yawOffset
		// It's the same for y and pitchOffset
		double xOffset = mouse_last_x - current_x;
		double yOffset = mouse_last_y - current_y;
		mouse_last_x = current_x;
		mouse_last_y = current_y;

		// Multiplication with the mouse sensitivity gives us a means of manipulating camera speed
		cam.updateYawAndPitch((float)(xOffset * mouseSensitivity), (float)(yOffset * mouseSensitivity));
	}
}

// Callback function
// xOffset and yOffset hold the scroll wheel's offset since the last callback
// xOffset is horizontal offset, yOffset is vertical offset
void Input::processScrollwheel(GLFWwindow* window, double xOffset, double yOffset) {
	// Fetch camera
	Camera& cam = ResourceManager::giveCamera();
	
	float fov = cam.fov;
	// We subtract the offset because scrolling forward increases the yOffset, but zooming inwards means a lower FOV value
	fov -= (float)yOffset;
	// Set limits to the zoom level so that we don't get weird flips at 0° and 90°
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