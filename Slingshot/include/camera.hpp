#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	glm::vec3 cameraPosition;
	glm::vec3 cameraDirectionVector;
	glm::vec3 cameraUpVector;
	glm::vec3 cameraRightVector;
	float fov;
	float cameraSpeed, rollSpeed;
	float mouseSensitivity;

	Camera();
	Camera(glm::vec3 givenCameraPosition, glm::vec3 givenCameraTarget, float givenCameraSpeed, float givenRollSpeed);

	void updateYawAndPitch(float yawOffset, float pitchOffset);
	void updateRotation(float rollOffset);
	glm::mat4 calculateViewMatrix();
};