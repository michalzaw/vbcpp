#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;
layout (location = 2) in vec3 VertexNormal;
#ifdef NORMALMAPPING
layout (location = 3) in vec3 VertexTangent;
layout (location = 4) in vec3 VertexBitangent;
#endif

const int CASCADES_COUNT = 3;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform mat4 LightSpaceMatrix[CASCADES_COUNT];

#ifdef GRASS
uniform sampler2D heightmap;
uniform sampler2D grassDensity;
uniform vec3 min;
uniform int width;
float TERRAIN_MAX_HEIGHT = 20;
#endif

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;
#ifdef NORMALMAPPING
	out vec3 TangentWorldspace;
	out vec3 BitangentWorldspace;
#endif
out vec4 PositionLightSpace[CASCADES_COUNT];
out float ClipSpacePositionZ;

int Random(int Seed, int Iterations)
{
	int Value = Seed;
	for (int i = 0; i < Iterations; ++i)
	{
		Value = ((Value >> 7) ^ (Value << 9)) * 15485863;
		//Value = Value * 15485863;
	}
	return Value;
}

void main()
{
	
	vec3 Offset = vec3(mod(gl_InstanceID, width) * 0.5, 0.0f, gl_InstanceID / width * 0.5f);
	Offset += min;
	Offset.y = 0;
	Offset.x += float(Random(int(Offset.z * 512 + Offset.x), 3) & 0xFF) / 512.0;
	Offset.z += float(Random(int(Offset.z * 512 + Offset.x), 2) & 0xFF) / 512.0;
	if (VertexUV.y <= -0.5f)
	{
		Offset.y -= float(Random(int(Offset.z * 512 + Offset.x), 2) & 0xFF) / 1024.0;
	}
	
	float h = texture2D(heightmap, vec2((Offset.x / 256.0f * 0.5f) + 0.5f, (Offset.z / 256.0f * 0.5f) + 0.5f)).r * TERRAIN_MAX_HEIGHT;
	Offset.y += h;
	//vec3 vertexPositionWithOffset = VertexPosition + Offset;
	
	float angle = float(Random(int(Offset.z * 512 + Offset.x), 4) & 0xFF) / 41.0f; // 255 / 41 ~= 2 * pi = 360
	float sina = sin(angle);
	float cosa = cos(angle);
	
	mat4 translationMatrix = mat4(1, 0, 0, 0,
								  0, 1, 0, 0,
								  0, 0, 1, 0,
								  Offset.x, Offset.y, Offset.z, 1);
							   
	mat4 rotationMatrix = mat4(cosa, 0, -sina, 0,
							   0, 1, 0, 0,
							   sina, 0, cosa, 0,
							   0, 0, 0, 1);
		
	vec3 vertexPositionWithRotationAndOffset = (translationMatrix * rotationMatrix * vec4(VertexPosition, 1.0f)).xyz;
	
	float density = texture2D(grassDensity, vec2((Offset.x / 256.0f * 0.5f) + 0.5f, (Offset.z / 256.0f * 0.5f) + 0.5f)).r;
	if (density < 0.2)
		vertexPositionWithRotationAndOffset.y = 0;
	
	gl_Position = MVP * vec4(vertexPositionWithRotationAndOffset, 1.0f);

	Position = (ModelMatrix * vec4(vertexPositionWithRotationAndOffset, 1.0f)).xyz;
	TexCoord = VertexUV;

#ifdef SOLID
	Normal = (NormalMatrix /* rotationMatrix */ * vec4(0.0f, 1.0f, 0.0f, 0.0f)).xyz;
#endif
#ifdef NORMALMAPPING
	mat3 NM = mat3(NormalMatrix);
	Normal = NM * VertexNormal;
	TangentWorldspace = NM * VertexTangent;
	BitangentWorldspace = NM * VertexBitangent;
#endif
	for (int i = 0; i < 3; ++i)
	{
		PositionLightSpace[i] = LightSpaceMatrix[i] * ModelMatrix * vec4(vertexPositionWithRotationAndOffset, 1.0f);
	}
	
	ClipSpacePositionZ = gl_Position.z;
}