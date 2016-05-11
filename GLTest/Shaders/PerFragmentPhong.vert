#version 330 core

#define MAX_INFLUENCES 4

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColour;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 vertexTexCoord;
layout (location = 4) in ivec4 boneIds; //4 ints max
layout (location = 5) in vec4 boneWeights; //4 floats max

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
	// Put the verts into position and THEN skin them //TODO could do this in software on batching and remove the need for the model matrix (and thus have many less batches)
	//vec4 skinnedPosition = weightedBoneMatrix * modelMatrix * vec4(vertexPosition, 1.0);

	vec4 worldPosition = modelMatrix * vec4(vertexPosition, 1.0); // Put the verts into position and THEN skin them
	mat4 weightedBoneMatrix = boneMatrixPalette[boneIds[0]] * boneWeights[0]
							+ boneMatrixPalette[boneIds[1]] * boneWeights[1]
							+ boneMatrixPalette[boneIds[2]] * boneWeights[2]
							+ boneMatrixPalette[boneIds[3]] * boneWeights[3];

	vec4 skinnedPosition = weightedBoneMatrix * worldPosition;

	// Convert to eye coordinates
	position = vec3(viewMatrix * skinnedPosition);
	normal = normalize(mat3(weightedBoneMatrix) * normalMatrix * vertexNormal);
	textureCoord = vertexTexCoord;
	gl_Position = projectionMatrix * viewMatrix * skinnedPosition;

}
