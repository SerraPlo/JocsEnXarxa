//The fragment shader operates on each pixel in a given polygon
#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 camera;
uniform mat4 model;

void main()
{
    gl_Position = camera * model * vec4(vertexPosition, 1.0);
    fragPosition = vec3(model * vec4(vertexPosition, 1.0f));
    fragNormal = mat3(transpose(inverse(model))) * vertexNormal;  
}