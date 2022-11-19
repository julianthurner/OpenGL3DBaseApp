// Remove later
#include <iostream>

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "camera.hpp"

//** Private **//

//** Public **//
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
	// We'll use quaternions as those save at least 50% computation time over Euler rotations
	// For the yaw, we rotate the camera around the up vector and update the now changed right vector
	glm::quat yawRotationQuaternion = glm::angleAxis(glm::radians(yawOffset), cameraUpVector);
	glm::mat4 yawRotationMatrix = glm::toMat4(yawRotationQuaternion);
	cameraRightVector = glm::vec3(yawRotationMatrix * glm::vec4(cameraRightVector, 1.0f));

	// For the pitch, we rotate the camera around the updated right vector and update the now changed up vector
	glm::quat pitchRotationQuaternion = glm::angleAxis(glm::radians(pitchOffset), cameraRightVector);
	glm::mat4 pitchRotationMatrix = glm::toMat4(pitchRotationQuaternion);
	cameraUpVector = glm::vec3(pitchRotationMatrix * glm::vec4(cameraUpVector, 1.0f));

	// Lastly, we update the direction vector
	glm::quat combinedQuaternion = yawRotationQuaternion * pitchRotationQuaternion;
	glm::mat4 combinedRotationMatrix = glm::toMat4(combinedQuaternion);
	// Normalization ensures that forward / backward movement always has the same speed
	cameraDirectionVector = glm::normalize(glm::vec3(combinedRotationMatrix * glm::vec4(cameraDirectionVector, 1.0f)));

	// Due to this kind of free flight camera implementation, the view will rotate if you look around in circles
	// Looking 90° up, then 90° left and then 90° down again will roll the camera counter-clockwise by 90°
	// This effect is normal for free flight cameras and if you want to discard it, you have to fix the camera in place
	// E.g. have the up vector always point toward positive y and prevent the user from looking too far down / up (-> FPS cam)
}

// Rotates the camera by the amount given
void Camera::updateRotation(float rollOffset) {
	// We'll use quaternions as those save at least 50% computation time over Euler rotations
	// First, we calculate a rotation matrix that rotates our camera around its direction vector
	glm::quat rotationQuaternion = glm::angleAxis(glm::radians(rollOffset), cameraDirectionVector);
	glm::mat4 rotationMatrix = glm::toMat4(rotationQuaternion);
	// Then we update both the up vector and the right vector
	cameraUpVector = glm::vec3(rotationMatrix * glm::vec4(cameraUpVector, 1.0f));
	cameraRightVector = glm::vec3(rotationMatrix * glm::vec4(cameraRightVector, 1.0f));
}

glm::mat4 Camera::calculateViewMatrix() {
	// First parameter of glm::lookAt is the camera position the second one the target position, the third one the up vector
	return glm::lookAt(cameraPosition, cameraPosition + cameraDirectionVector, cameraUpVector);
}