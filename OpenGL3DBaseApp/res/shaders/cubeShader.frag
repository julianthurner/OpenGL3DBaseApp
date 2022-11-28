#version 330 core
in vec2 vertexTexturePosition;

// For good reason, we start numbering at 0
// This way, our numbers will match up with OpenGL's which reduces room for errors
uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float blendValue;

out vec4 fragColor;

void main() {
	// The mix function blends the textures by using the modifier given as third argument.
	// texture() assisgns a texture to the specified texture coordinates
	// The higher the value, the higher the influence of the second texture
	fragColor = mix(
		texture(texture0, vertexTexturePosition),
		texture(texture1, vertexTexturePosition),
		blendValue);
}