#version 330 core

const int CASCADES_COUNT = 3;
const float CascadeEndClipSpace[CASCADES_COUNT] = float[CASCADES_COUNT](25.0f, 100.0f, 500.0f);
//const float bias[CASCADES_COUNT] = float[CASCADES_COUNT](0.00005f, 0.0005f, 0.005f);
const float bias[CASCADES_COUNT] = float[CASCADES_COUNT](0.0004f, 0.0008f, 0.005f);
const float size[CASCADES_COUNT] = float[CASCADES_COUNT](2048.0f, 1024.0f, 512.0f);

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

uniform samplerCube Texture;
uniform vec3 CameraPosition;
uniform sampler2D NormalmapTexture;

out vec4 Color;

const float Air = 1.0;
const float Glass = 1.51714;
const float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));




struct Light
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
	Light Base;
	vec3 Direction;
};

struct Attenuation
{
	float constant;
	float linear;
	float exp;
};

struct PointLight
{
	Light Base;
	vec3 Position;
	Attenuation Attenuation;
};

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float CutoffCos;
};

const int MAX_DIR_COUNT = 1;
const int MAX_POINT_COUNT = 8;
const int MAX_SPOT_COUNT = 8;



uniform LightsBlock
{
	DirectionalLight 	DirLights[MAX_DIR_COUNT];
	PointLight 			PointLights[MAX_POINT_COUNT];
	SpotLight 			SpotLights[MAX_SPOT_COUNT];
	int DirCount;
	int PointCount;
	int SpotCount;
} Lights;

in vec4 PositionLightSpace[CASCADES_COUNT];
in float ClipSpacePositionZ;
uniform sampler2DShadow ShadowMap[CASCADES_COUNT];


vec4 CalculateLight(Light l, vec3 normal, vec3 dir)
{
	vec3 FragmentToEye = normalize(CameraPosition - Position);
	vec3 LightReflect = normalize(reflect(dir, normal));
	float SpecularFactor = max(dot(FragmentToEye, LightReflect), 0.0f);
	SpecularFactor = pow(SpecularFactor, 96);
	vec4 SpecularColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * SpecularFactor;
	
	return SpecularColor;
}


vec4 CalculateDiffuseLight(Light l, vec3 normal, vec3 dir, float ratio)
{
	vec4 AmbientColor = vec4(l.Color, 1.0f) * l.AmbientIntensity;
	
	float DiffuseFactor = max(dot(normal, -dir), 0.0f);
	vec4 DiffuseColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * DiffuseFactor;
	
	return (AmbientColor + DiffuseColor * ratio);// * textureColor;
}

vec4 CalculateDiffuseLight2(Light l, vec3 normal, vec3 dir, float ratio)
{
	vec4 AmbientColor = vec4(l.Color, 1.0f) * l.AmbientIntensity;
	
	float DiffuseFactor = max(dot(normal, -dir), 0.0f);
	vec4 DiffuseColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * DiffuseFactor;
	
	return (DiffuseColor * ratio);// * textureColor;
}



void main()
{
	int cascadeIndex = 0;
	for (int i = 0; i < CASCADES_COUNT; ++i)
	{
		if (ClipSpacePositionZ <= CascadeEndClipSpace[i])
		{
			cascadeIndex = i;
			break;
		}
	}
	//cascadeIndex = 0;
	
	// Shadows
	float Ratio = 1.0f;
	vec3 Coords = PositionLightSpace[cascadeIndex].xyz / PositionLightSpace[cascadeIndex].w;
	Coords = Coords * 0.5f + 0.5f;

	//float Depth = texture(ShadowMap[cascadeIndex], Coords.xy).r;
	float CurrentDepth = Coords.z;

	Coords.z -= bias[cascadeIndex];//0.0005f;//
	Ratio = texture(ShadowMap[cascadeIndex], Coords);//CurrentDepth - 0.0005f > Depth ? 0.5f : 1.0f;//
	Ratio = Ratio * 0.8f + 0.2f;
	
	
	
	
	vec3 vector = normalize(vec3(Position - CameraPosition));
	
	vec3 reflection = reflect(vector, Normal);
	
	float fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-vector, Normal)), 0.5);
	
	vec4 specular = CalculateLight(Lights.DirLights[0].Base, Normal, Lights.DirLights[0].Direction);
	vec4 diffuse = CalculateDiffuseLight(Lights.DirLights[0].Base, Normal, Lights.DirLights[0].Direction, 1);
	vec4 diffuse2 = CalculateDiffuseLight2(Lights.DirLights[0].Base, -Normal, Lights.DirLights[0].Direction, Ratio);
	
	//Color.rgb = mix(texture(Texture, normalize(reflection)).rgb * 0.3, vec3(0.0f, 0.0f, 0.0f), fresnel);
	//Color.rgb = mix(texture(Texture, normalize(reflection)).rgb * 0.15, vec3(0.0f, 0.0f, 0.0f), fresnel);
	
	float reflectionValue = texture2D(NormalmapTexture, TexCoord).r;
	//Color.rgb = texture(Texture, normalize(reflection)).rgb * 0.2 + specular.rgb;//0.5
	Color.rgb = texture(Texture, normalize(reflection)).rgb * 0.3 * reflectionValue * diffuse.rgb + specular.rgb;//0.5
	//Color.rgb = mix(texture(Texture, normalize(reflection)).rgb * 0.2, texture(Texture, normalize(reflection)).rgb, fresnel) + specular.rgb;
	//Color.a = mix(0.6f - diffuse2.r, 1.0f - diffuse2.r, fresnel);//0.3
	//Color.a = mix(0.6f, 1.0f, fresnel);//0.3//Oryginalna alpha, na pewno dziala
	float q = mix(0.3f, 0.6f, reflectionValue);
	Color.a = mix(q, mix(q, 1.0f, reflectionValue), fresnel);//0.3
}
