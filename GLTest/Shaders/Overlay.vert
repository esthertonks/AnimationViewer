#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColour;

out vec3 colour;

uniform float pointSize;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix; //TODO precalc some of these multiplications

void main()
{
	colour = vertexColour;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	gl_PointSize = pointSize;
}
