#version 330 core
layout (location = 0) in vec3 givenPosition;
layout (location = 1) in vec3 givenColor;
  
out vec3 vertexColor;

void main() {
    gl_Position = vec4(givenPosition, 1.0);
    vertexColor = givenColor;
}