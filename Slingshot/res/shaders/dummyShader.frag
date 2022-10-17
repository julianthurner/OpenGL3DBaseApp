#version 330 core
in vec3 vertexColor;
in vec2 vertexTexturePos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float blendValue;

out vec4 fragColor;

void main() {
	// The mix function blends the textures by using the modifier given as third argument.
	// The higher the value, the higher the influence of the second texture
	fragColor = mix(texture(texture1, vertexTexturePos), texture(texture2, vec2(1 - vertexTexturePos.x, vertexTexturePos.y)), blendValue);
}