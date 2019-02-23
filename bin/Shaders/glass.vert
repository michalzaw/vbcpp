#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;
layout (location = 2) in vec3 VertexNormal;

const int CASCADES_COUNT = 3;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform mat4 LightSpaceMatrix[CASCADES_COUNT];

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;

out vec4 PositionLightSpace[CASCADES_COUNT];
out float ClipSpacePositionZ;

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 1.0f);
	
	Position = (ModelMatrix * vec4(VertexPosition, 1.0f)).xyz;
	Normal = (NormalMatrix * vec4(VertexNormal, 0.0f)).xyz;
	
	for (int i = 0; i < 3; ++i)
	{
		PositionLightSpace[i] = LightSpaceMatrix[i] * ModelMatrix * vec4(VertexPosition, 1.0f);
	}
	
	ClipSpacePositionZ = gl_Position.z;
	
	TexCoord = VertexUV;
}