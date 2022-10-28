#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	glm::vec3 cameraPosition;
	glm::vec3 cameraDirectionVector;
	glm::vec3 cameraRightVector;
	glm::vec3 cameraUpVector;
	float fov;
	float cameraSpeed;

	Camera(glm::vec3 givenCameraPosition, glm::vec3 givenCameraTarget, float givenCameraSpeed);

	void updateCameraVectors();
	glm::mat4 calculateViewMatrix();
};