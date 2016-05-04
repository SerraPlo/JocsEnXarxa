//The fragment shader operates on each pixel in a given polygon
#version 330 core

in vec2 texCoords;

out vec4 color;

// Texture samplers
uniform sampler2D texture_diffuse;

void main() {
	// Linearly interpolate between both textures (second texture is only slightly combined)
	color = texture(texture_diffuse, texCoords);
}