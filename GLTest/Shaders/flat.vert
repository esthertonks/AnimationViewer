#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;

flat out vec3 lightIntensity;

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

uniform Light light;
uniform Material material;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix; //TODO precalc some of these multiplications
uniform mat3 normalMatrix;

void main()
{
	// Convert to eye coordinates
	vec4 eyeCoords = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	vec3 eyeNormal = normalize(normalMatrix * vertexNormal);

	vec3 lightDirection = normalize(vec3(light.position - eyeCoords));
	vec3 viewDirection = normalize(-eyeCoords.xyz);

	vec3 reflection = reflect(-lightDirection, eyeNormal); // gl reflect function 

	vec3 ambient = light.ambient * material.ambient;

	float diffuseLightIntensity = max(dot(lightDirection, eyeNormal), 0.0);

	vec3 diffuse = light.diffuse * material.diffuse * diffuseLightIntensity;

	vec3 specular = vec3(0.0);

	if(diffuseLightIntensity > 0)
	{
		specular = light.specular * material.specular * pow(max(dot(reflection, viewDirection), 0.0), material.shininess);
	}

	lightIntensity = ambient + diffuse + specular;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

}
