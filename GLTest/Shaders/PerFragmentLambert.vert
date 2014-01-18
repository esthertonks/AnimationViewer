#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColour;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 vertexTexCoord;
layout (location = 4) in vec4 boneIds;
layout (location = 5) in vec4 boneWeights;

out vec3 position;
out vec3 normal;
out vec3 colour;
out vec2 textureCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix; //TODO precalc some of these multiplications
uniform mat3 normalMatrix;

uniform mat4 boneMatrixPalette[128]; // TODO Max 128 bones per batch - split if more than 128

void main()
{
	// Skin the verts
	//vec4 skinnedPosition = vec4(0.0, 0.0, 0.0, 1.0);

	// for(int weightIndex = 0; weightIndex < 4; weightIndex++)
	//{
		//if(boneWeights[weightIndex] > 0.0)
		//{
			//skinnedPosition += boneWeights[weightIndex] * boneMatrixPalette[int(boneIds[weightIndex])] * vec4(vertexPosition, 1.0);
		//}
	// }
	mat4 weightedBoneMatrix = //boneMatrixPalette[int(boneIds[2])] * boneWeights[1];
							boneMatrixPalette[int(boneIds[0])] * boneWeights[0]
							+ boneMatrixPalette[int(boneIds[1])] * boneWeights[1]
							+ boneMatrixPalette[int(boneIds[2])] * boneWeights[2]
							+ boneMatrixPalette[int(boneIds[3])] * boneWeights[3];

	vec4 skinnedPosition = weightedBoneMatrix * vec4(vertexPosition, 1.0);

	// Convert to eye coordinates
	position = vec3(viewMatrix * modelMatrix * skinnedPosition);
	normal = normalize(normalMatrix * vertexNormal);
	colour = vec3(boneMatrixPalette[1][0][0], 1.0, 1.0);
	textureCoord = vertexTexCoord;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * skinnedPosition;

}
