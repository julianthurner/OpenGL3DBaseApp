#version 330 core
layout (location = 0) in vec3 givenPos;
layout (location = 1) in vec3 givenColor;
layout (location = 2) in vec2 givenTexturePos;

uniform mat4 transformMatrix;

out vec3 vertexColor;
out vec2 vertexTexturePos;

void main() {
	gl_Position = transformMatrix * vec4(givenPos, 1.0f);
	vertexColor = givenColor;
	vertexTexturePos = givenTexturePos;
}