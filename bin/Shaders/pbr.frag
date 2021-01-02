#version 330

in vec2 TexCoord;
in vec3 n;
in vec3 Position;
in vec3 TangentWorldspace;
in vec3 BitangentWorldspace;

uniform sampler2D AlbedoTexture;
uniform sampler2D NormalmapTexture;
uniform sampler2D MetalicTexture;
uniform sampler2D RoughnessTexture;
uniform sampler2D AoTexture;

uniform samplerCube IrradianceMap;
uniform samplerCube SpecularIrradianceMap;
uniform sampler2D brdfLUT;

uniform vec3 CameraPosition;

out vec3 Color;


// Lights
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


// Shadows
const int CASCADES_COUNT = 3;
const float CascadeEndClipSpace[CASCADES_COUNT] = float[CASCADES_COUNT](25.0f, 100.0f, 500.0f);
const float bias[CASCADES_COUNT] = float[CASCADES_COUNT](0.0004f, 0.0008f, 0.005f);

in vec4 PositionLightSpace[CASCADES_COUNT];
in float ClipSpacePositionZ;
uniform sampler2DShadow ShadowMap[CASCADES_COUNT];


const float PI = 3.14159265359;


float DistributionGGX(vec3 N, vec3 H, float a)
{
	float a2     = a*a * a * a; // było a * a
	float NdotH  = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom    = a2;
	float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
	denom        = PI * denom * denom;

	return nom / denom;
}


float GeometrySchlickGGX(float NdotV, float k)
{
	float nom   = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);

	return ggx1 * ggx2;
}


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
} 


void main()
{
	vec3 viewDir = normalize(CameraPosition - Position);
	
	
	//vec3 normal = normalize(n);
	vec3 n1 = normalize((texture2D(NormalmapTexture, TexCoord).rgb * 2.0f - 1.0f) * vec3(1.0f, 1.0f, 1.0f));
	
	mat3 TBN = mat3(normalize(TangentWorldspace),
					normalize(BitangentWorldspace),
					n);
	
	vec3 normal = normalize(TBN * n1);
	
	
	// Material
	vec3 albedo = pow(texture(AlbedoTexture, TexCoord).rgb, vec3(2.2));//vec3(1.00, 0.0, 0.0);//
	float metalic = texture(MetalicTexture, TexCoord).r;//1.0f;
	float roughness = texture(RoughnessTexture, TexCoord).r;//a;//0.1f;
	float ao = texture(AoTexture, TexCoord).r;//1.0f;
	
	if (texture(AlbedoTexture, TexCoord).a < 0.5)
		discard;
	
	vec3 f0 = vec3(0.04f);
	f0 = mix(f0, albedo, metalic);
	
	
	vec3 L0 = vec3(0.0f, 0.0f, 0.0f);
	// Radiancja
	for (int i = 0; i < Lights.DirCount; ++i)
	{
		vec3 lightColor = Lights.DirLights[i].Base.Color * Lights.DirLights[i].Base.DiffuseIntensity * 1.9;
		vec3 lightDir = -normalize(Lights.DirLights[i].Direction);
		
		float cosTheta = max(dot(normal, lightDir), 0.0f);
		
		float attenuation = 1.0f;
#ifdef SHADOWMAPPING
		int cascadeIndex = 0;
		for (int i = 0; i < CASCADES_COUNT; ++i)
		{
			if (ClipSpacePositionZ <= CascadeEndClipSpace[i])
			{
				cascadeIndex = i;
				break;
			}
		}

		vec3 Coords = PositionLightSpace[cascadeIndex].xyz / PositionLightSpace[cascadeIndex].w;
		Coords = Coords * 0.5f + 0.5f;

		float CurrentDepth = Coords.z;

		Coords.z -= bias[cascadeIndex];//0.0005f;//
		attenuation = texture(ShadowMap[cascadeIndex], Coords);//CurrentDepth - 0.0005f > Depth ? 0.5f : 1.0f;//
#endif
		vec3 radiance = lightColor * attenuation;
	
	// Specular
	vec3 h = normalize(viewDir + lightDir);
	
	vec3 F = fresnelSchlick(max(dot(h, viewDir), 0.0f), f0); // wedlug tutoriala zamiast normal powinno byc h, ale wtedy nie wyglada to jak efekt fresnela
	
	float NDF = DistributionGGX(normal, h, roughness);
	float G = GeometrySmith(normal, viewDir, lightDir, (roughness + 1) * (roughness + 1) / 8.0f);
	
	vec3 specular = (NDF * G * F) / max((4.0f * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0)), 0.001);
	
	
	vec3 ks = F;
	vec3 kd = vec3(1.0f) - ks;
	kd *= 1.0f - metalic;
	
	
	// L0
	L0 += (kd * albedo / PI + specular) * radiance * cosTheta;
	}
	
	
	//vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 f = fresnelSchlickRoughness(max(dot(normal, viewDir), 0.0f), f0, roughness);
	
	vec3 ambient_kS = f;
	vec3 ambient_kD = vec3(1.0f) - ambient_kS;
	ambient_kD *= 1.0f - metalic;
	vec3 irradiance = texture(IrradianceMap, normal).rgb;
	vec3 diffuse = irradiance * albedo;
	


	
	vec3 r = reflect(-viewDir, normal);
	
	const float MAX_REFLECTION_DOT = 5.0f;
	vec3 prefilteredColor = textureLod(SpecularIrradianceMap, r, roughness * MAX_REFLECTION_DOT).rgb;
	vec2 envBRDF = texture(brdfLUT, vec2(max(dot(normal, viewDir), 0.0f), 1 - roughness)).rg;
	vec3 ambient_specular = prefilteredColor * (f0 * envBRDF.x + envBRDF.y);
	
	
	
	vec3 ambient = (ambient_kD * diffuse + ambient_specular) * ao;// * 0.25;
	
	
	
	Color = ambient + L0;
}