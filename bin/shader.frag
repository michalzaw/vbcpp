#version 330 core

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


in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragmentColor;


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
uniform float Transparency;
uniform sampler2D Texture;
uniform float SpecularPower;

uniform vec3 CameraPosition;


vec4 CalculateLight(Light l, vec3 normal, vec3 dir)
{
	vec4 AmbientColor = vec4(l.Color, 1.0f) * l.AmbientIntensity;
	
	float DiffuseFactor = max(dot(normal, -dir), 0.0f);
	vec4 DiffuseColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * DiffuseFactor;
	
	vec3 FragmentToEye = normalize(CameraPosition - Position);
	vec3 LightReflect = normalize(reflect(dir, normal));
	float SpecularFactor = max(dot(FragmentToEye, LightReflect), 0.0f);
//	SpecularFactor = pow(SpecularFactor, SpecularPower);
	SpecularFactor = pow(SpecularFactor, 96);
	vec4 SpecularColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * SpecularFactor;
	
	return (AmbientColor + DiffuseColor + SpecularColor);
}


vec4 CalculatePointLight(PointLight light, vec3 normal)
{
	vec3 Direction = Position - light.Position;
	float Distance = length(Direction);
	float Attenuation = light.Attenuation.constant + light.Attenuation.linear * Distance +
						light.Attenuation.exp * Distance * Distance;
						
	return CalculateLight(light.Base, normal, normalize(Direction)) / Attenuation;
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


void main()
{
	vec3 n = normalize(Normal);
	
	vec4 LightsColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < Lights.DirCount; ++i)
	{
		LightsColor += CalculateLight(Lights.DirLights[i].Base, n, Lights.DirLights[i].Direction);
	}
	
	for (int i = 0; i < Lights.PointCount; ++i)
	{
		LightsColor += CalculatePointLight(Lights.PointLights[i], n);
	}
	
	for (int i = 0; i < Lights.SpotCount; ++i)
	{
		LightsColor += CalculateSpotLight(Lights.SpotLights[i], n);
	}

	//vec4 amb = AmbientColor * matAmbient;
	//vec4 diff = DiffuseColor * matDiffuse;
	//vec4 spec = SpecularColor * matSpecular;
	
	//FragmentColor = (AmbientColor + DiffuseColor + SpecularColor);
	//FragmentColor = texture2D(Texture, TexCoord) * (amb + diff + spec);
	FragmentColor = texture2D(Texture, TexCoord) * LightsColor;
	
	FragmentColor.a = 1 - Transparency;
}
