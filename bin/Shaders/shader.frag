#version 330 core

const int CASCADES_COUNT = 2;
const float CascadeEndClipSpace[CASCADES_COUNT] = float[CASCADES_COUNT](25.0f, 1000.0f);
//const float bias[CASCADES_COUNT] = float[CASCADES_COUNT](0.00005f, 0.0005f, 0.005f);
const float bias[CASCADES_COUNT] = float[CASCADES_COUNT](0.0004f, 0.0008f);
const float size[CASCADES_COUNT] = float[CASCADES_COUNT](2048.0f, 1024.0f);

#ifdef REFLECTION
const float Air = 1.0;
const float Glass = 1.51714;
const float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));
#endif


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
#ifdef DECALS
in vec4 ClipSpacePosition;
#endif

//out vec4 FragmentColor;
layout (location = 0) out vec4 FragmentColor;
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
#ifdef GRASS
uniform vec4 grassColor;
uniform vec4 grassFraction;
uniform sampler2D NoiseTexture;
#endif
#ifdef REFLECTION
uniform samplerCube environmentMap;
uniform samplerCube environmentMap2;
uniform mat4 environmentMap1Rotation;
uniform mat4 environmentMap2Rotation;
#endif
#ifdef GLASS
uniform sampler2D glassTexture;
uniform float dayNightRatio;
#endif
#ifdef EMISSIVE
uniform sampler2D emissiveTexture;
#endif
#ifdef DECALS
uniform sampler2D depthMap;
uniform mat4 viewProjMatrixInv;
uniform mat4 modelMatrixInv;
#endif

uniform vec3 CameraPosition;

uniform sampler2DArrayShadow ShadowMap;

uniform sampler2D t[7];
uniform int grassTexturesCount;
flat in int tIndex;

#ifdef RENDER_OBJECT_ID
uniform uint objectId;
#endif

vec4 textureColor;
vec4 ambient;
vec4 diffuse;
vec4 specular;

vec3 Position;


vec4 CalculateLight(Light l, vec3 normal, vec3 dir, float ratio)
{
	vec4 AmbientColor = vec4(l.Color, 1.0f) * l.AmbientIntensity;// * 4; // dla aces * 4 a powinno być / 2, dla classic / 16
	
	float DiffuseFactor = max(dot(normal, -dir), 0.0f);
	vec4 DiffuseColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * DiffuseFactor;// * 2;// * 20;
	
	vec3 FragmentToEye = normalize(CameraPosition - Position);
	vec3 LightReflect = normalize(reflect(dir, normal));
	float SpecularFactor = max(dot(FragmentToEye, LightReflect), 0.0f);
	SpecularFactor = pow(SpecularFactor, SpecularPower);
//	SpecularFactor = pow(SpecularFactor, 96);
	vec4 SpecularColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * SpecularFactor;
	
	return AmbientColor * ambient + (DiffuseColor * diffuse + SpecularColor * specular) * ratio;
}


vec4 CalculatePointLight(PointLight light, vec3 normal)
{
	vec3 Direction = Position - light.Position;
	float Distance = length(Direction);
	float Attenuation = light.Attenuation.constant + light.Attenuation.linear * Distance +
						light.Attenuation.exp * Distance * Distance;
						
	return CalculateLight(light.Base, normal, normalize(Direction), 1) / Attenuation;
}


vec4 CalculateSpotLight(SpotLight light, vec3 normal)
{
	vec3 LightToPixel = normalize(Position - light.Base.Position);
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


#ifdef DECALS
// https://stackoverflow.com/questions/32227283/getting-world-position-from-depth-buffer-value
vec4 depthToWorldPosition(float depth, vec2 depthUV)
{
    float z = depth * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(depthUV * 2.0 - 1.0, z, 1.0);
    vec4 worldSpacePosition = viewProjMatrixInv * clipSpacePosition;
    worldSpacePosition /= worldSpacePosition.w;

    return worldSpacePosition;
}


vec4 getDecalColor(vec4 positionWorldSpace)
{
	vec4 positionLocalSpace = modelMatrixInv * positionWorldSpace;
	if (abs(positionLocalSpace.x) < 0.5f && abs(positionLocalSpace.y) < 0.5f && abs(positionLocalSpace.z) < 0.5f)
	{
		vec4 color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		return texture2D(Texture, positionLocalSpace.xz + vec2(0.5f));
	}
	else
	{
		return vec4(0.0f, 1.0f, 0.0f, 0.0f);
	}
}
#endif


void main()
{
	Position = PositionVert;

	vec3 normal = normalize(Normal);
	
#ifdef NORMALMAPPING
	vec3 n = normalize(texture2D(NormalmapTexture, TexCoord).rgb * 2.0f - 1.0f);
	
	mat3 TBN = mat3(normalize(TangentWorldspace),
					normalize(BitangentWorldspace),
					normal);
	
	normal = normalize(TBN * n);
#endif
	
#ifdef SOLID
	textureColor = texture2D(Texture, TexCoord);
	float gamma = 2.2;
	textureColor.rgb = pow(textureColor.rgb, vec3(gamma));
	ambient = textureColor;
	diffuse = textureColor;
	specular = matSpecular * textureColor;
#endif

#ifdef DECALS
	vec2 positionScreenSpace = ClipSpacePosition.xy / ClipSpacePosition.w;
	vec2 depthUV = positionScreenSpace * vec2(0.5f, 0.5f) + vec2(0.5f, 0.5f);
	float depth = texture(depthMap, depthUV).r;
	vec4 positionWorldSpace = depthToWorldPosition(depth, depthUV);

	textureColor = getDecalColor(positionWorldSpace);
	textureColor.rgb = pow(textureColor.rgb, vec3(gamma));
	ambient = textureColor;
	diffuse = textureColor;
	specular = matSpecular * textureColor;
	
	Position = positionWorldSpace.xyz;

	// https://github.com/ColinLeung-NiloCat/UnityURPUnlitScreenSpaceDecalShader/blob/master/URP_NiloCatExtension_ScreenSpaceDecal_Unlit.shader
	vec3 dx = dFdx(positionWorldSpace.xyz);
	vec3 dy = dFdy(positionWorldSpace.xyz);
	normal = normalize(cross(dx, dy));
#endif

#ifdef GLASS
	vec3 vector = normalize(vec3(Position - CameraPosition));
	vec3 reflection = reflect(vector, normal);
	
	float reflectionValue = texture2D(glassTexture, TexCoord).r;
	float t = reflectionValue;
	reflectionValue = (reflectionValue * dayNightRatio - 0.5f * dayNightRatio + 0.5f) * 0.7 + 0.3;

	ambient.rgb = mix(texture(environmentMap, mat3(environmentMap1Rotation) * normalize(reflection)).rgb * matDiffuse.rgb * reflectionValue,
					  texture(environmentMap2, mat3(environmentMap2Rotation) * normalize(reflection)).rgb * matDiffuse.rgb * reflectionValue,
					  1 - t);
	diffuse = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
#endif
	
float isGrass = 0.0f;
#ifdef GRASS
	float c = (textureColor.r + textureColor.g + textureColor.b) / 3.0f;
	//textureColor = diffuse = vec4(c, c, c, textureColor.a) * grassColor;

	textureColor = texture2D(t[tIndex], TexCoord);
	textureColor.rgb = pow(textureColor.rgb, vec3(gamma));

	if (tIndex == grassTexturesCount - 1)
	textureColor = diffuse = ambient = textureColor * grassColor;
	else
	textureColor = diffuse = ambient = textureColor;
	//vec4 noseValue = texture2D(NoiseTexture, TexCoord);
	//float distanceToCamera = (ClipSpacePositionZ - 25) / 5.0f;
	//if (distanceToCamera > 0 && texture2D(NoiseTexture, TexCoord).r <= distanceToCamera)
	//	discard;
	isGrass = 1.0f;
#endif
	float miFactor = 0;
	float normalFactor = 1;
#ifdef ALPHA_TEST
	if (textureColor.a < 0.1f)
		discard;
#endif
	
#ifdef SUBSURFACE_SCATTERING
	ambient /= 4.0f;
	//diffuse /= 2.0f;

	vec3 eyeToFramgent = normalize(Position - CameraPosition);
	vec3 lightDir = Lights.DirLights[0].Direction;
	
	miFactor = max(dot(-lightDir, eyeToFramgent), 0.0f);
	
	miFactor = mix(miFactor, 0, isGrass);
	
	diffuse.rgb = mix(diffuse.rgb, 1 * vec3(diffuse.g * 0.9, diffuse.g * 1.0, diffuse.g * 0.2), miFactor);
	//diffuse.rgb = mix(diffuse.rgb, 1.5 * diffuse.rgb, miFactor);
	//ambient.rgb = mix(ambient.rgb, 4 * vec3(ambient.g * 0.9, ambient.g * 1.0, ambient.g * 0.2), miFactor);
	
	float DiffuseFactor = dot(normal, -lightDir);
	//if (miFactor > 0.0f)
	normalFactor = mix(1, -1, miFactor);
		normal = normalFactor * normal;
#endif

	
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
		//cascadeIndex = 0;
		
		// Shadows
		float Ratio = 1.0f;
#ifdef SHADOWMAPPING
		Ratio = 0.0f;
		vec3 Coords = PositionLightSpace[cascadeIndex].xyz / PositionLightSpace[cascadeIndex].w;
		Coords = Coords * 0.5f + 0.5f;

		//float Depth = texture(ShadowMap[cascadeIndex], Coords.xy).r;
		float CurrentDepth = Coords.z;

		Coords.z -= bias[cascadeIndex];//0.0005f;//

		// only hardware 2x2 PCF
		Ratio = texture(ShadowMap, vec4(Coords.xy, cascadeIndex, Coords.z));//CurrentDepth - 0.0005f > Depth ? 0.5f : 1.0f;//

		// 4x4 PCF
		/*vec2 TexelSize = 1.0f / textureSize(ShadowMap[cascadeIndex], 0) / 2.0f;
		for (float i = -1.5; i <= 1.5; ++i) {
			for (float j = -1.5; j <= 1.5; ++j) {
				Ratio += texture(ShadowMap[cascadeIndex], Coords + vec3(i * TexelSize.x, j * TexelSize.y, 0.0f));//CurrentDepth - 0.0005f > Depth ? 0.5f : 1.0f;//
			}
		}

		Ratio /= 16.0f;*/

		if (normalFactor >= 0)
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

	//vec4 amb = AmbientColor * matAmbient;
	//vec4 diff = DiffuseColor * matDiffuse;
	//vec4 spec = SpecularColor * matSpecular;
	
	//FragmentColor = (AmbientColor + DiffuseColor + SpecularColor);
	//FragmentColor = texture2D(Texture, TexCoord) * (amb + diff + spec);
	
	FragmentColor.rgb = LightsColor.rgb;
	
#ifdef CAR_PAINT
	vec3 vector = normalize(vec3(Position - CameraPosition));
	
	vec3 reflection = reflect(vector, normal);
	
	float fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-vector, normal)), 1);
	
	
	vec3 FragmentToEye = normalize(CameraPosition - Position);
	float vvv = clamp(dot(normal, FragmentToEye), 0.0f, 1.0f);
	
	//FragmentColor.rgb = texture(env, normalize(reflection)).rgb * (1.0f - 0.9f * vvv) + 0.8f * LightsColor.rgb;
	fresnel = mix(0.2, 0.5, fresnel);
	FragmentColor.rgb = mix(0.8 * LightsColor.rgb, texture(environmentMap, normalize(reflection)).rgb * LightsColor.rgb, fresnel);
	//FragmentColor.rgb = LightsColor.rgb;
	//FragmentColor.rgb = 0.1f * texture(env, normalize(reflection)).rgb * (1.0f - 0.5f * vvv) + (1 - fresnel) * LightsColor.rgb;
#endif
	FragmentColor.a = 1.0f;
	
#ifdef GLASS
	float fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-vector, normal)), matDiffuse.a);
	
	float q = mix(Transparency * 0.0f, Transparency, reflectionValue);
	FragmentColor.a = mix(q, mix(q, 1.0f, reflectionValue), fresnel);//0.3
#endif
#ifdef ALPHA_TEST
	FragmentColor.a = textureColor.a;
#endif

	FragmentColor += matEmissive * textureColor;
#ifdef EMISSIVE
	FragmentColor += texture2D(emissiveTexture, TexCoord);
#endif
	
#ifdef ALPHA_TEST
	float _Cutoff = 0.5f;// dla wysokiego drzewa lepsze jest 0.2, dla drobnych lisci 0.4, a było na poczatku 0.3, w ostatniej wersji było 0.2, zmienione na potrzeby trawy. TODO: sterowac z meterialu
	float newAlpha = (FragmentColor.a - _Cutoff) / max(fwidth(FragmentColor.a), 0.0001) + 0.5;
	
	FragmentColor.a = mix(FragmentColor.a, newAlpha, fixDisappearanceAlphaRatio);
#endif



	float brightness = dot(FragmentColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 0.1f)
		BrightnessColor = vec4(FragmentColor.rgb, FragmentColor.a);
	else
		BrightnessColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	

#ifdef RENDER_OBJECT_ID
	ObjectIdValue = uvec4(objectId, 0, 0, 0);
#endif
}
