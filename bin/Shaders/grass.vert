#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;
layout (location = 2) in vec3 VertexNormal;
#ifdef NORMALMAPPING
layout (location = 3) in vec3 VertexTangent;
layout (location = 4) in vec3 VertexBitangent;
#endif

const int CASCADES_COUNT = 2;

uniform mat4 VP;
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

out vec4 PositionLightSpace[CASCADES_COUNT];
out float ClipSpacePositionZ;

int Random(int Seed, int Iterations)
{
	int Value = Seed;
	for (int i = 0; i < Iterations; ++i)
	{
		Value = ((Value >> 7) ^ (Value << 9)) * 15485863;
	}
	return Value;
}

void main()
{
	vec2 terrainSize = textureSize(heightmap, 0);
	
	vec3 Offset = vec3(mod(gl_InstanceID, width) * 0.5, 0.0f, gl_InstanceID / width * 0.5f);
	Offset += min;
	int grassIndex = int(Offset.z * terrainSize.x + Offset.x);
	Offset.y = 0;
	Offset.x += float(Random(grassIndex, 3) & 0xFF) / 1024.0;
	Offset.z += float(Random(grassIndex, 2) & 0xFF) / 1024.0;
	if (VertexUV.y <= -0.5f)
	{
		Offset.y -= float(Random(grassIndex, 2) & 0xFF) / 1024.0;
	}
	
	vec4 heightNormal = texture2D(heightmap, vec2((Offset.x / (terrainSize.x * 0.5f) * 0.5f) + 0.5f, (Offset.z / (terrainSize.y * 0.5f) * 0.5f) + 0.5f));
	float h = heightNormal.r * TERRAIN_MAX_HEIGHT;
	Offset.y += h;
	
	float angle = float(Random(grassIndex, 4) & 0xFF) / 41.0f; // 255 / 41 ~= 2 * pi = 360
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
		
	vec3 vertexPositionWithRotationAndOffset = (translationMatrix * rotationMatrix * ModelMatrix * vec4(VertexPosition, 1.0f)).xyz;
	
	float density = texture2D(grassDensity, vec2((Offset.x / 256.0f * 0.5f) + 0.5f, (Offset.z / 256.0f * 0.5f) + 0.5f)).r;
	if (density < 0.2)
		vertexPositionWithRotationAndOffset.y = 0;
	
	gl_Position = VP * vec4(vertexPositionWithRotationAndOffset, 1.0f);

	Position = (/*ModelMatrix */ vec4(vertexPositionWithRotationAndOffset, 1.0f)).xyz;
	TexCoord = VertexUV;

#ifdef SOLID
	Normal = vec3((heightNormal.g - 0.5) * 2, (heightNormal.b - 0.5) * 2, (heightNormal.a - 0.5) * 2);
	//Normal = (NormalMatrix * vec4(Normal, 0.0f)).xyz;
#endif

	for (int i = 0; i < CASCADES_COUNT; ++i)
	{
		PositionLightSpace[i] = LightSpaceMatrix[i] /* ModelMatrix*/ * vec4(vertexPositionWithRotationAndOffset, 1.0f);
	}
	
	ClipSpacePositionZ = gl_Position.z;
}