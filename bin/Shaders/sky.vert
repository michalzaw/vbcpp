#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 MVP;
uniform mat4 ModelMatrix;

out vec3 TexCoord;

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 1.0f);

	TexCoord = normalize(VertexPosition);
}