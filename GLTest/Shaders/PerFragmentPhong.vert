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
	mat4 weightedBoneMatrix = mat4(1.0);

	for(int weightIndex = 0; weightIndex < 4; weightIndex++) // Put the verts into position and THEN skin them //TODO could do this in software on batching and remove the need for the model matrix (and thus have many less batches)
	{
		if(boneWeights[weightIndex] > 0.0)
		{
			weightedBoneMatrix += boneMatrixPalette[int(boneIds[weightIndex])] * boneWeights[weightIndex];
		}
	 }

	 // Put the verts into position and THEN skin them //TODO could do this in software on batching and remove the need for the model matrix (and thus have many less batches)
	vec4 skinnedPosition = weightedBoneMatrix * modelMatrix * vec4(vertexPosition, 1.0);

	// Convert to eye coordinates
	position = vec3(viewMatrix * skinnedPosition);
	normal = normalize(normalMatrix * vertexNormal);
	//colour = vec3(weightedBoneMatrix[1][0], 1.0, 1.0);
	colour = vec3(boneWeights[0], boneWeights[1], boneWeights[2]);
	textureCoord = vertexTexCoord;
	gl_Position = projectionMatrix * viewMatrix * skinnedPosition;

}
