#version 330 core

in vec3 colour;

layout (location = 0) out vec4 fragColor;

void main() 
{
	fragColor = vec4(colour, 1.0);
}