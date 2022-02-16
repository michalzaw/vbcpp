#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;
layout (location = 2) in vec3 VertexNormal;
#ifdef NORMALMAPPING
layout (location = 3) in vec3 VertexTangent;
layout (location = 4) in vec3 VertexBitangent;
#endif

const int CASCADES_COUNT = 2;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform mat4 LightSpaceMatrix[CASCADES_COUNT];

uniform float time;

out vec3 PositionVert;
out vec2 TexCoord;
out vec3 Normal;
#ifdef NORMALMAPPING
	out vec3 TangentWorldspace;
	out vec3 BitangentWorldspace;
#endif
out vec4 PositionLightSpace[CASCADES_COUNT];
out float ClipSpacePositionZ;
#ifdef DECALS
out vec4 ClipSpacePosition;
#endif

void main()
{
	vec3 pos = VertexPosition;
#ifdef TREE
	//pos.x += ((2 * sin(4 * (pos.x + pos.y + pos.z + time))) + 1) * 0.05;
	//pos.y += ((1 * sin(8 * (pos.x + pos.y + pos.z + time))) + 0.5) * 0.05;
	//pos.z += ((1 * sin(8 * (pos.x + pos.y + pos.z + time))) + 0.5) * 0.05;

	//pos += (0.065 * sin(2.650 * (pos.x + pos.y + pos.z + time))) * vec3(1, 0.35, 1);
#endif

	gl_Position = MVP * vec4(pos, 1.0f);

	PositionVert = (ModelMatrix * vec4(pos, 1.0f)).xyz;
	TexCoord = VertexUV;

//#ifdef SOLID
	Normal = (NormalMatrix * vec4(VertexNormal, 0.0f)).xyz;
//#endif
#ifdef NORMALMAPPING
	mat3 NM = mat3(NormalMatrix);
	Normal = NM * VertexNormal;
	TangentWorldspace = NM * VertexTangent;
	BitangentWorldspace = NM * VertexBitangent;
#endif
	for (int i = 0; i < CASCADES_COUNT; ++i)
	{
		PositionLightSpace[i] = LightSpaceMatrix[i] * ModelMatrix * vec4(pos, 1.0f);
	}
	
	ClipSpacePositionZ = gl_Position.z;

#ifdef DECALS
	ClipSpacePosition = gl_Position;
#endif
}