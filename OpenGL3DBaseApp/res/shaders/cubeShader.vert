#version 330 core
layout (location = 0) in vec3 givenPosition;
layout (location = 1) in vec2 givenTexturePosition;

uniform mat4 modelMatrix;

// Uniform buffer object (UBO) that stores matrices that can be shared between shaders
layout (std140) uniform matrices {
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

// vertexColor isn't actually used anymore, I left it in here for possible later use
out vec3 vertexColor;
out vec2 vertexTexturePosition;

void main() {
	// Follows the classic OpenGL Model-View-Projection-Matrix style
	// Remember that matrix multiplications are read from right to left
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(givenPosition, 1.0f);
	vertexTexturePosition = givenTexturePosition;
}