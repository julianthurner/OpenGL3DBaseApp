#version 330 core
layout (location = 0) in vec3 givenPos;
layout (location = 1) in vec3 givenColor;
layout (location = 2) in vec2 givenTexturePos;

out vec3 vertexColor;
out vec2 vertexTexturePos;

void main() {
	gl_Position = vec4(givenPos, 1.0);
	vertexColor = givenColor;
	vertexTexturePos = givenTexturePos;
}