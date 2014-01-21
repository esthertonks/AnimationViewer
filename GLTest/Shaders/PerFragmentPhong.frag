#version 330

in vec3 position;
in vec3 normal;
in vec3 colour;
in vec2 textureCoord;

uniform sampler2D diffuseTexture;

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
	float diffuseFactor; // Interpolation amount with ambient. Must be 0-1
};

uniform Light light;
uniform Material material;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix; //TODO precalc some of these multiplications
uniform mat3 normalMatrix;

void PhongShading(in vec3 materialDiffuse, out vec3 ambient, out vec3 diffuse, out vec3 specular)
{
	vec3 lightDirection = normalize(vec3(light.position) - position);
	vec3 viewDirection = normalize(-position.xyz);

	vec3 reflection = reflect(-lightDirection, normalize(normal)); // gl reflect function 

	ambient = light.ambient * material.ambient;

	float diffuseLightIntensity = max(dot(lightDirection, normal), 0.0);

	diffuse = light.diffuse * materialDiffuse * diffuseLightIntensity;

	specular = vec3(0.0);

	if(diffuseLightIntensity > 0)
	{
		specular = light.specular * material.specular * pow(max(dot(reflection, viewDirection), 0.0), material.shininess);
	}
}

//http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 HSVtoRGB(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 RGBtoHSV(vec3 c)
{
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

float Interpolate(float start, float end, float value)
{
	return start * (1.0 - value) + end * value;
}

vec3 InterpolateColour(vec3 colourA, vec3 colourB, float amount) // Amount must be a value between 0.0 and 1.0
{
	vec3 hsvColourA = RGBtoHSV(colourA);
	vec3 hsvColourB = RGBtoHSV(colourB);
	vec3 hsvResult;

	hsvResult.x = Interpolate(hsvColourA.x, hsvColourB.x, amount); // H
	hsvResult.y = Interpolate(hsvColourA.y, hsvColourB.y, amount); // S
	hsvResult.z = Interpolate(hsvColourA.z, hsvColourB.z, amount); // V

	return HSVtoRGB(hsvResult);
}

//vec3 Hue(float hue)
//{
//	float red = abs(hue * 6 - 3) - 1;
//	float green = 2 - abs(hue * 6 - 2);
//	float blue = 2 - abs(hue * 6 - 4);

//	vec3 rgb;
//	rgb.r = clamp(red, 0.0, 1.0);
//	rgb.g = clamp(green, 0.0, 1.0);
//	rgb.b = clamp(blue, 0.0, 1.0);
//	return rgb;
//}

//vec3 HSVtoRGB(in vec3 hsv)
//{
//	return vec3(((Hue(hsv.x) - 1) * hsv.y + 1) * hsv.z);
//}

vec3 InterpolateVector(vec3 start, vec3 end, float value)
{
	return start * (1.0 - value) + end * value;
}

void main() 
{
	vec4 textureColour = texture(diffuseTexture, textureCoord);
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// when a diffuse texture is attached in maya it overrides any diffuse parameter (which will be loads in as zero anyway)
	// There is ALWAYS a texture added on import, so assume that there is one
	PhongShading(vec3(textureColour), ambient, diffuse, specular);

	//vec3 ambientAndDiffuse = InterpolateColour(ambient, diffuse, material.diffuseFactor);
	//fragmentColour = (vec4(ambientAndDiffuse, 1.0) * textureColour) + vec4(specular, 1.0);

	vec3 ambientAndDiffuse = InterpolateVector(ambient, diffuse, material.diffuseFactor);
//	fragmentColour = vec4(ambientAndDiffuse, 1.0) + vec4(specular, 1.0);
	fragmentColour = vec4(colour, 1.0);
}
