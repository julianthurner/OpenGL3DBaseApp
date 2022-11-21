#version 330 core
layout (location = 0) in vec4 givenPos;

// We don't need a model matrix, the plane never changes
layout (std140) uniform matrices {
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

void main() {
	gl_Position = projectionMatrix * viewMatrix * givenPos;
}