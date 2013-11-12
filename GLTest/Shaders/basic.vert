#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;

out vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	color = vertexColor;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition,1.0);
}
