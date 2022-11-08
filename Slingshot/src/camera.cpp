// Remove later
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"

Camera::Camera(glm::vec3 givenCameraPosition, glm::vec3 givenCameraTarget, float givenCameraSpeed, float givenRollSpeed) {
	cameraPosition = givenCameraPosition;
	// Note that we use a directionVector that is pointing *away* from the camera instead of towards
	// Normalization ensures that we always move at the same speed towards / away from the target
	cameraDirectionVector = glm::normalize(givenCameraTarget - cameraPosition);

	fov = 45.0f;
	cameraSpeed = givenCameraSpeed;
	rollSpeed = givenRollSpeed;
	mouseSensitivity = 0.1f;

	/// The up vector is the cross product of the direction vector and the right vector (which simply points towards positive x)
	// Normalization ensures that vertical strafing always has the same speed
	cameraUpVector = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), cameraDirectionVector));

	// We get the camera's right vector by taking the cross product of the camera's direction vector and up vector
	// Normalization ensures that horizontal strafing always has the same speed
	cameraRightVector = glm::normalize(glm::cross(cameraDirectionVector, cameraUpVector));
}

// Changes the camera's direction
void Camera::updateYawAndPitch(float yawOffset, float pitchOffset) {
	// Implement this
	double starttime = glfwGetTime();
	glm::quat yawRotationQuaternion2 =glm::angleAxis(glm::radians(yawOffset), cameraUpVector);
	glm::mat4 yawRotationMatrix3 = glm::toMat4(yawRotationQuaternion2);
	glm::vec3 cameraRightVector3 = glm::vec3(yawRotationMatrix3 * glm::vec4(cameraRightVector, 1.0f));
	glm::quat pitchRotationQuaternion2 = glm::angleAxis(glm::radians(pitchOffset), cameraRightVector3);
	glm::mat4 pitchRotationMatrix3 = glm::toMat4(pitchRotationQuaternion2);
	glm::vec3 cameraUpVector3 = glm::vec3(yawRotationMatrix3 * glm::vec4(cameraUpVector, 1.0f));
	glm::quat finalQuaternion = yawRotationQuaternion2 * pitchRotationQuaternion2;
	glm::mat4 finalMatrix = glm::toMat4(finalQuaternion);
	glm::vec3 cameraDirectionVector3 = glm::normalize(glm::vec3(finalMatrix * glm::vec4(cameraDirectionVector, 1.0f)));
	
	// For the yaw, we rotate the camera around the up vector and update the now changed right vector
	glm::mat4 yawRotationMatrix = glm::mat4(1.0f); // Start off with an identity matrix
	yawRotationMatrix = glm::rotate(yawRotationMatrix, glm::radians(yawOffset), cameraUpVector);
	cameraRightVector = glm::vec3(yawRotationMatrix * glm::vec4(cameraRightVector, 1.0f));

	// For the pitch, we rotate the camera around the updated right vector and update the now changed up vector
	glm::mat4 pitchRotationMatrix = glm::mat4(1.0f); // Reset the rotationMatrix to an identity matrix
	pitchRotationMatrix = glm::rotate(pitchRotationMatrix, glm::radians(pitchOffset), cameraRightVector);
	cameraUpVector = glm::vec3(pitchRotationMatrix * glm::vec4(cameraUpVector, 1.0f));

	// Lastly, we update the direction vector
	// Normalization ensures that forward / backward movement always has the same speed
	cameraDirectionVector = glm::normalize(glm::vec3(yawRotationMatrix * pitchRotationMatrix * glm::vec4(cameraDirectionVector, 1.0f)));
}

// Rotates the camera by the amount given
void Camera::updateRotation(float rollOffset) {
	// Implement this
	double starttime = glfwGetTime();
	glm::quat rotationQuaternion = glm::angleAxis(glm::radians(rollOffset), cameraDirectionVector);
	glm::mat4 rotationMatrix2 = glm::toMat4(rotationQuaternion);
	glm::vec3 newUpVector2 = glm::vec3(rotationMatrix2 * glm::vec4(cameraUpVector, 1.0f));
	glm::vec3 newRightVector2 = glm::vec3(rotationMatrix2 * glm::vec4(cameraRightVector, 1.0f));
	
	// First, we calculate a rotation matrix that rotates our camera around its direction vector
	glm::mat4 rotationMatrix = glm::mat4(1.0f); // Start off with identity matrix
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rollOffset), cameraDirectionVector);
	// Then we update voth the up vector and the right vector
	cameraUpVector = glm::vec3(rotationMatrix * glm::vec4(cameraUpVector, 1.0f));
	cameraRightVector = glm::vec3(rotationMatrix * glm::vec4(cameraRightVector, 1.0f));
}

glm::mat4 Camera::calculateViewMatrix() {
	// First parameter of glm::lookAt is the camera position the second one the target position, the third one the up vector
	return glm::lookAt(cameraPosition, cameraPosition + cameraDirectionVector, cameraUpVector);
}