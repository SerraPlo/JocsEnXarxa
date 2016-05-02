//The vertex shader operates on each vertex
#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;

out vec2 texCoords;

uniform mat4 camera;
uniform mat4 model;

void main() {
	gl_Position = camera * model * vec4(vertexPosition, 1.0f);
    texCoords = vertexUV;
}