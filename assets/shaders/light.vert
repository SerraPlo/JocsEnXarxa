//The fragment shader operates on each pixel in a given polygon
#version 330 core
 
layout(location=0) in vec3 vertPosition;
layout(location=1) in vec3 vertNormal;
layout(location=2) in vec2 vertUV;
 
out vec4 fragPosition; // Position in world space.
out vec4 fragNormal; // Surface normal in world space.
out vec2 fragUV;
 
// Model, View, Projection matrix
uniform mat4 camera;
uniform mat4 model;
 
void main()
{
    gl_Position = camera * model * vec4(vertPosition, 1);
 
    fragPosition = model * vec4(vertPosition, 1); 
    fragNormal = model * vec4(vertNormal, 0);
    fragUV = vertUV;
}