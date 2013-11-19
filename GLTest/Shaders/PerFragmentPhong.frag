#version 330

in vec3 position;
in vec3 normal;

layout (location = 0) out vec4 fragmentColour;

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

vec3 PhongShading()
{
	vec3 lightDirection = normalize(vec3(light.position) - position);
	vec3 viewDirection = normalize(-position.xyz);

	vec3 reflection = reflect(-lightDirection, normalize(normal)); // gl reflect function 

	vec3 ambient = light.ambient * material.ambient;

	float diffuseLightIntensity = max(dot(lightDirection, normal), 0.0);

	vec3 diffuse = light.diffuse * material.diffuse * diffuseLightIntensity;

	vec3 specular = vec3(0.0);

	if(diffuseLightIntensity > 0)
	{
		specular = light.specular * material.specular * pow(max(dot(reflection, viewDirection), 0.0), material.shininess);
	}

	return ambient + diffuse + specular;
}

void main() 
{
	fragmentColour = vec4(PhongShading(), 1.0);
}
