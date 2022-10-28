#include "camera.hpp"

Camera::Camera(glm::vec3 givenCameraPosition, glm::vec3 givenCameraTarget, float givenCameraSpeed) {
	cameraPosition = givenCameraPosition;
	// Note that we use a directionVector that is pointing *away* from the camera instead of towards
	cameraDirectionVector = givenCameraTarget - cameraPosition;
	fov = 45.0f;
	cameraSpeed = givenCameraSpeed;

	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	// Right vector is the normalized cross product of a normalized vector that points upwards and the camera's direction vector
	cameraRightVector = glm::normalize(glm::cross(cameraDirectionVector, glm::vec3(0.0f, 1.0f, 0.0f)));

	// Up vector is the normalized cross product of the direction vector and the right vector
	cameraUpVector = glm::normalize(glm::cross(cameraRightVector, cameraDirectionVector));
}

glm::mat4 Camera::calculateViewMatrix() {
	// First parameter of glm::lookAt is the camera position and the second one the target position
	// The third one is a simple vector that is pointing upwards
	return glm::lookAt(cameraPosition, cameraPosition + cameraDirectionVector, cameraUpVector);
}