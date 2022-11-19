#version 330 core
layout (location = 0) in vec4 givenPos;

// We don't need a model matrix, the plane never changes
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
	gl_Position = projectionMatrix * viewMatrix * givenPos;
}