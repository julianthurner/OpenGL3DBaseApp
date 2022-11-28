#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	glm::vec3 cameraPosition;
	glm::vec3 cameraDirectionVector;
	glm::vec3 cameraUpVector;
	glm::vec3 cameraRightVector;
	float fov;
	float moveSpeed, rollSpeed;
	float mouseSensitivity;

	Camera(const glm::vec3 cameraPosition, const glm::vec3 cameraTarget, const float moveSpeed, const float rollSpeed);

	void updateYawAndPitch(const float yawOffset, const float pitchOffset);
	void updateRotation(const float rollOffset);
	void updateViewMatrix() const;
	void updateProjectionMatrix() const;
};