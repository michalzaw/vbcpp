#version 330 core

in vec3 Position;
in vec3 Normal;

uniform samplerCube Texture;
uniform vec3 CameraPosition;

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



void main()
{
	vec3 vector = normalize(vec3(Position - CameraPosition));
	
	vec3 reflection = reflect(vector, Normal);
	
	float fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-vector, Normal)), 0.5);
	
	vec4 specular = CalculateLight(Lights.DirLights[0].Base, Normal, Lights.DirLights[0].Direction);
	vec4 diffuse = CalculateDiffuseLight(Lights.DirLights[0].Base, Normal, Lights.DirLights[0].Direction, 1);
	
	//Color.rgb = mix(texture(Texture, normalize(reflection)).rgb * 0.3, vec3(0.0f, 0.0f, 0.0f), fresnel);
	//Color.rgb = mix(texture(Texture, normalize(reflection)).rgb * 0.15, vec3(0.0f, 0.0f, 0.0f), fresnel);
	
	
	//Color.rgb = texture(Texture, normalize(reflection)).rgb * 0.2 + specular.rgb;//0.5
	Color.rgb = texture(Texture, normalize(reflection)).rgb * 0.2 + specular.rgb;//0.5
	//Color.rgb = mix(texture(Texture, normalize(reflection)).rgb * 0.2, texture(Texture, normalize(reflection)).rgb, fresnel) + specular.rgb;
	Color.a = mix(0.6f, 1.0f, fresnel);//0.3
}
