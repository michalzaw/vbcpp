#version 330 core

const int CASCADES_COUNT = 2;
const float CascadeEndClipSpace[CASCADES_COUNT] = float[CASCADES_COUNT](25.0f, 1000.0f);
//const float bias[CASCADES_COUNT] = float[CASCADES_COUNT](0.00005f, 0.0005f, 0.005f);
const float bias[CASCADES_COUNT] = float[CASCADES_COUNT](0.0004f, 0.0008f);
const float size[CASCADES_COUNT] = float[CASCADES_COUNT](2048.0f, 1024.0f);

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


in vec3 PositionVert;
in vec2 TexCoord;
in vec3 Normal;
#ifdef NORMALMAPPING
in vec3 TangentWorldspace;
in vec3 BitangentWorldspace;
#endif
in vec4 PositionLightSpace[CASCADES_COUNT];
in float ClipSpacePositionZ;

//out vec4 FragmentColor;
layout (location = 0) out vec3 FragmentColor;
layout (location = 1) out vec4 BrightnessColor;
#ifdef RENDER_OBJECT_ID
layout (location = 2) out uvec4 ObjectIdValue;
#endif


uniform LightsBlock
{
	DirectionalLight 	DirLights[MAX_DIR_COUNT];
	PointLight 			PointLights[MAX_POINT_COUNT];
	SpotLight 			SpotLights[MAX_SPOT_COUNT];
	int DirCount;
	int PointCount;
	int SpotCount;
} Lights;


uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform vec4 matEmissive;
uniform float Transparency;
uniform float fixDisappearanceAlphaRatio;
uniform sampler2D Texture;
uniform float SpecularPower;
#ifdef NORMALMAPPING
uniform sampler2D NormalmapTexture;
#endif

uniform vec3 CameraPosition;

uniform sampler2DShadow ShadowMap[CASCADES_COUNT];

vec4 textureColor;
vec4 ambient;
vec4 diffuse;
vec4 specular;

uniform vec3 color1;
uniform vec3 color2;
uniform vec3 color3;
uniform vec3 color4;

#ifdef RENDER_OBJECT_ID
uniform uint objectId;
#endif


vec4 CalculateLight(Light l, vec3 normal, vec3 dir, float ratio)
{
	vec4 AmbientColor = vec4(l.Color, 1.0f) * l.AmbientIntensity;// dla aces / 2, dla classic / 16
	
	float DiffuseFactor = max(dot(normal, -dir), 0.0f);
	vec4 DiffuseColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * DiffuseFactor;// * 2;// * 20;
	
	vec3 FragmentToEye = normalize(CameraPosition - PositionVert);
	vec3 LightReflect = normalize(reflect(dir, normal));
	float SpecularFactor = max(dot(FragmentToEye, LightReflect), 0.0f);
	SpecularFactor = pow(SpecularFactor, SpecularPower);
//	SpecularFactor = pow(SpecularFactor, 96);
	vec4 SpecularColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * SpecularFactor;
	
	return AmbientColor * ambient + (DiffuseColor * diffuse + SpecularColor * specular) * ratio;
}


vec4 CalculatePointLight(PointLight light, vec3 normal)
{
	vec3 Direction = PositionVert - light.Position;
	float Distance = length(Direction);
	float Attenuation = light.Attenuation.constant + light.Attenuation.linear * Distance +
						light.Attenuation.exp * Distance * Distance;
						
	return CalculateLight(light.Base, normal, normalize(Direction), 1) / Attenuation;
}


vec4 CalculateSpotLight(SpotLight light, vec3 normal)
{
	vec3 LightToPixel = normalize(PositionVert - light.Base.Position);
	float SpotFactor = dot(LightToPixel, light.Direction);
	
	if (SpotFactor > light.CutoffCos)
	{
		vec4 LightColor = CalculatePointLight(light.Base, normal);
		return LightColor * (1.0f - (1.0f - SpotFactor) * 1.0f / (1.0f - light.CutoffCos));
	}
	else
	{
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}


void main()
{
	vec3 diffuseColor = color1;
	vec3 subsurfaceColor = color2;
	
	vec3 normal = normalize(Normal);
#ifdef NORMALMAPPING
	vec3 n = normalize(texture2D(NormalmapTexture, TexCoord).rgb * 2.0f - 1.0f);
	
	mat3 TBN = mat3(normalize(TangentWorldspace),
					normalize(BitangentWorldspace),
					normal);
	
	normal = normalize(TBN * n);
#endif
	
	/*vec2 tex_offset = 1.0 / textureSize(Texture, 0);
    vec4 result = vec4(0, 0, 0, 0);//texture(texture1, texCoord).rgb;
	
	for(int i = -1; i < 2; ++i)
	{
		for(int j = -1; j < 2; ++j)
		{
			result += texture(Texture, TexCoord + vec2(tex_offset.y * i, tex_offset.y * j));
		}
	}
	
	vec4 textureColor = result / 9.0f;*/
	
	vec4 textureColor = texture(Texture, TexCoord);
	
	float gamma = 2.2;
	textureColor.rgb = pow(textureColor.rgb, vec3(gamma));
	
	
	ambient = matAmbient * textureColor;
	diffuse = matDiffuse * textureColor;
	specular = matSpecular * textureColor;
	
	if (textureColor.a < 0.2f)
		discard;
	/*---------------------------------------------------------------*/
	/*
	Lepsza alfa z poprawianiem zanikania wraz z odleglosica. Aby dobrze wygladalo nalezy w materiale drzewa ustawic parametr ambient na 1,1,1,1 zamiast obecnego 0,0,0,1
	float alpha = textureColor.a;
	float _Cutoff = 0.3f;// dla wysokiego drzewa lepsze jest 0.2, dla drobnych lisci 0.4, a było na poczatku 0.3, w ostatniej wersji było 0.2, zmienione na potrzeby trawy. TODO: sterowac z meterialu
	float newAlpha = (alpha - _Cutoff) / max(fwidth(alpha), 0.0001) + 0.5;
	
	float distance = length(CameraPosition - PositionVert);

	alpha = mix(alpha, newAlpha, distance / 100.0f);
	// mozna tez dac _Cutoff = 0.2 i wtedy distance / 200.0f, ale to sie trzeba zastanowic


	if (alpha < 0.75f)
		discard;

	//FragmentColor.a = alpha;
	*/
	/*---------------------------------------------------------------*/
	
	
	
	
	/*vec3 eyeToFramgent = normalize(PositionVert - CameraPosition);
	vec3 lightDir = Lights.DirLights[0].Direction;
	
	float miFactor = max(dot(-lightDir, eyeToFramgent), 0.0f);
	
	//miFactor = mix(miFactor, 0, isGrass);
	
	diffuse.rgb = mix(diffuse.rgb, 1 * vec3(diffuse.g * 0.9, diffuse.g * 1.0, diffuse.g * 0.2), miFactor);
	//diffuse.rgb = mix(diffuse.rgb, 1.5 * diffuse.rgb, miFactor);
	//ambient.rgb = mix(ambient.rgb, 4 * vec3(ambient.g * 0.9, ambient.g * 1.0, ambient.g * 0.2), miFactor);
	
	float DiffuseFactor = dot(normal, -lightDir);
	//if (miFactor > 0.0f)
	float normalFactor = mix(1, -1, miFactor);
		normal = normalFactor * normal;*/
		
	vec3 eyeToFramgent = normalize(PositionVert - CameraPosition);
	vec3 lightDir = Lights.DirLights[0].Direction;
	float dot1 = dot(normal, eyeToFramgent);
	float dot2 = dot(normal, lightDir);
	if (sign(dot1) != sign(dot2))
	{
		normal = -normal;
		diffuse.rgb *= subsurfaceColor;
	}
	else
	{
		diffuse.rgb *= diffuseColor;
	}
	
	
	
	
	vec4 LightsColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < Lights.DirCount; ++i)
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
		
		// Shadows
		float Ratio = 1.0f;
#ifdef SHADOWMAPPING
		vec3 Coords = PositionLightSpace[cascadeIndex].xyz / PositionLightSpace[cascadeIndex].w;
		Coords = Coords * 0.5f + 0.5f;

		float CurrentDepth = Coords.z;

		Coords.z -= bias[cascadeIndex];
		Ratio = texture(ShadowMap[cascadeIndex], Coords);//CurrentDepth - 0.0005f > Depth ? 0.5f : 1.0f;//
		//if (normalFactor >= 0)
		Ratio = Ratio * 0.8f + 0.2f;
#endif
	
		LightsColor += CalculateLight(Lights.DirLights[i].Base, normal, Lights.DirLights[i].Direction, Ratio);
	}
	
	for (int i = 0; i < Lights.PointCount; ++i)
	{
		LightsColor += CalculatePointLight(Lights.PointLights[i], normal);
	}

	for (int i = 0; i < Lights.SpotCount; ++i)
	{
		LightsColor += CalculateSpotLight(Lights.SpotLights[i], normal);
	}
	
	FragmentColor.rgb = LightsColor.rgb;
	
	FragmentColor.rgb += (matEmissive * textureColor).rgb;

	
#ifdef RENDER_OBJECT_ID
	ObjectIdValue = uvec4(objectId, 0, 0, 0);
#endif
}
