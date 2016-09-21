#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec3 VertexTangent;
layout (location = 4) in vec3 VertexBitangent;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;
out vec3 TangentWorldspace;
out vec3 BitangentWorldspace;

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 1.0f);

	Position = (ModelMatrix * vec4(VertexPosition, 1.0f)).xyz;
	TexCoord = VertexUV;
	//Normal = (NormalMatrix * vec4(VertexNormal, 0.0f)).xyz;
	
	mat3 NM = mat3(NormalMatrix);
	Normal = NM * VertexNormal;
	TangentWorldspace = NM * VertexTangent;
	BitangentWorldspace = NM * VertexBitangent;
}