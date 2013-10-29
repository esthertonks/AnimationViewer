#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexTexCoord;

out vec3 TexCoord;

uniform mat4 RotationMatrix;

void main()
{
    TexCoord = VertexTexCoord;

    gl_Position = RotationMatrix * vec4(VertexPosition,1.0);
}
