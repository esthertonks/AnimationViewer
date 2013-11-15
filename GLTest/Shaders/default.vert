#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;

out vec3 lightIntensity;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform vec4 lightPosition;
uniform vec3 Kd; // Reflection coefficient - fraction of incoming scattered light (diffuse reflectivity)
uniform vec3 Ld; // Intesity of the light source

void main()
{
	// Convert to eye coordinates
	vec4 eyeCoords = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	vec3 eyeNormal = normalize(normalMatrix * vertexNormal);

	vec3 lightDirection = normalize(vec3(lightPosition - eyeCoords));

	lightIntensity = Ld * Kd * max(dot(lightDirection, eyeNormal), 0.0);

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

}
