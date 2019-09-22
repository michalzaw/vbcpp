#version 330 core
const int CASCADES_COUNT = 3;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec3 VertexTangent;
layout (location = 4) in vec3 VertexBitangent;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform mat4 LightSpaceMatrix[CASCADES_COUNT];

uniform vec3 CameraPosition;

out vec2 TexCoord;
out vec3 n;
out vec3 Position;
out vec3 TangentWorldspace;
out vec3 BitangentWorldspace;

out vec4 PositionLightSpace[CASCADES_COUNT];
out float ClipSpacePositionZ;

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 1.0f);
	Position = (ModelMatrix * vec4(VertexPosition, 1.0f)).xyz;

	TexCoord = vec2(1 * UV.x, 1 * UV.y);
	
	mat3 NM = mat3(NormalMatrix);
	n = NM * VertexNormal;
	TangentWorldspace = NM * VertexTangent;
	BitangentWorldspace = NM * VertexBitangent;
	
	for (int i = 0; i < 3; ++i)
	{
		PositionLightSpace[i] = LightSpaceMatrix[i] * ModelMatrix * vec4(VertexPosition, 1.0f);
	}
	
	ClipSpacePositionZ = gl_Position.z;
}