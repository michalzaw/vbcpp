#version 330 core

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragmentColor;

struct DirectionalLight
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	vec3 Direction;
};
uniform DirectionalLight Light;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;

uniform float Transparency;
uniform sampler2D Texture;
uniform float SpecularPower;

uniform vec3 CameraPosition;

uniform float a;


void main()
{
	vec3 n = normalize(Normal);

	vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;
	
	float DiffuseFactor = max(dot(normalize(Normal), -Light.Direction), 0.0f);
	vec4 DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;

	vec3 FragmentToEye = normalize(CameraPosition - Position);
	vec3 LightReflect = normalize(reflect(Light.Direction, normalize(Normal)));
	float SpecularFactor = max(dot(FragmentToEye, LightReflect), 0.0f);
//	SpecularFactor = pow(SpecularFactor, SpecularPower);
	SpecularFactor = pow(SpecularFactor, 96);
	vec4 SpecularColor = vec4(Light.Color, 1.0f) * SpecularFactor;
	
	//FragmentColor = texture2D(Texture, TexCoord) * (AmbientColor + DiffuseColor + SpecularColor);

	vec4 amb = AmbientColor * matAmbient;
	vec4 diff = DiffuseColor * matDiffuse;
	vec4 spec = SpecularColor * matSpecular;
	
	//FragmentColor = (AmbientColor + DiffuseColor + SpecularColor);
	FragmentColor = texture2D(Texture, TexCoord) * (amb + diff + spec);
	
	FragmentColor.a = 1 - Transparency;
}
