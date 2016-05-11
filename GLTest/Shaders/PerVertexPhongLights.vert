#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;

out vec3 lightIntensity;

struct Light // Light Intensity
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material // Reflectivity and shininess
{
	vec3 ambient;	// Ambient reflectivity
	vec3 diffuse;	// Diffuse reflectivity
	vec3 specular;	// Specular reflectivity
	float shininess;
};

uniform Light lights[1];
uniform Material material;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix; //TODO precalc some of these multiplications
uniform mat3 normalMatrix;

vec3 PhongLighting(
	int lightIndex,
	vec4 eyeCoords,
	vec3 eyeNormal,
	vec3 viewDirection
	)
{
	vec3 lightDirection = normalize(vec3(lights[lightIndex].position - eyeCoords));
	vec3 reflection = reflect(-lightDirection, eyeNormal); // gl reflect function 

	vec3 ambient = lights[lightIndex].ambient * material.ambient;

	float diffuseLightIntensity = max(dot(lightDirection, eyeNormal), 0.0);

	vec3 diffuse = lights[lightIndex].diffuse * material.diffuse * diffuseLightIntensity;

	vec3 specular = vec3(0.0);

	if(diffuseLightIntensity > 0)
	{
		specular = lights[lightIndex].specular * material.specular * pow(max(dot(reflection, viewDirection), 0.0), material.shininess);
	}

	return ambient + diffuse + specular;
}

// TODO can move the light calc out of the shader and do it once...
void main()
{
	// Convert to eye coordinates
	vec4 eyeCoords = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	vec3 eyeNormal = normalize(normalMatrix * vertexNormal);

	vec3 viewDirection = normalize(-eyeCoords.xyz);

	lightIntensity = vec3(0.0);
	for(int lightIndex = 0; lightIndex < 1; lightIndex++) //TODO pass in the number surely?
	{
		lightIntensity += PhongLighting(lightIndex, eyeCoords, eyeNormal, viewDirection); 
	}

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}

