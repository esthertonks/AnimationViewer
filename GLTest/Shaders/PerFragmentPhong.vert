#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColour;
layout (location = 2) in vec3 vertexNormal;

out vec3 position;
out vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix; //TODO precalc some of these multiplications
uniform mat3 normalMatrix;

void main()
{
	// Convert to eye coordinates
	position = vec3(viewMatrix * modelMatrix * vec4(vertexPosition, 1.0));
	normal = normalize(normalMatrix * vertexNormal);

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

}
