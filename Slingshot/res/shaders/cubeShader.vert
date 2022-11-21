#version 330 core
layout (location = 0) in vec3 givenPos;
layout (location = 1) in vec2 givenTexturePos;

uniform mat4 modelMatrix;

layout (std140) uniform matrices {
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

out vec3 vertexColor;
out vec2 vertexTexturePos;

void main() {
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(givenPos, 1.0f);
	vertexTexturePos = givenTexturePos;
}