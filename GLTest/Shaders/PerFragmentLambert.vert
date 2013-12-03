#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColour;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 vertexTexCoord;

out vec3 position;
out vec3 normal;
out vec3 colour;
out vec2 textureCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix; //TODO precalc some of these multiplications
uniform mat3 normalMatrix;

void main()
{
	// Convert to eye coordinates
	position = vec3(viewMatrix * modelMatrix * vec4(vertexPosition, 1.0));
	normal = normalize(normalMatrix * vertexNormal);
	colour = vertexColour;
	textureCoord = vertexTexCoord;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

}
