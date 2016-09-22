#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec3 VertexTangent;
layout (location = 4) in vec3 VertexBitangent;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

out vec3 PositionWorldspace;
out vec2 TexCoord;
out vec3 NormalWorldspace;
out vec3 TangentWorldspace;
out vec3 BitangentWorldspace;

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 1.0f);

	PositionWorldspace = (ModelMatrix * vec4(VertexPosition, 1.0f)).xyz;
	TexCoord = VertexUV;
	
	mat3 NM = mat3(NormalMatrix);
	NormalWorldspace = NM * VertexNormal;
	TangentWorldspace = NM * VertexTangent;
	BitangentWorldspace = NM * VertexBitangent;
}