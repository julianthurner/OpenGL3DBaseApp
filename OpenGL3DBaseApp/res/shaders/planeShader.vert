#version 330 core
layout (location = 0) in vec4 givenPosition;

// Uniform buffer object (UBO) that stores matrices that can be shared between shaders
// We don't need a model matrix, the plane never changes
layout (std140) uniform matrices {
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

void main() {
	// Remember that matrix multiplications are read from right to left
	gl_Position = projectionMatrix * viewMatrix * givenPosition;
}