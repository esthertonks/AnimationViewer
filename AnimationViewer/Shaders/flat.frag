#version 330 core

flat in vec3 lightIntensity;

layout (location = 0) out vec4 fragColor;

void main() 
{
	fragColor = vec4(lightIntensity, 1.0);
}