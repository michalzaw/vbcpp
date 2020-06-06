#version 330 core

in vec3 TexCoord;

uniform samplerCube Texture;
//uniform sampler2D Texture;
uniform float dayNightRatio;

//out vec4 Color;
layout (location = 0) out vec4 Color;
layout (location = 1) out vec4 BrightnessColor;  


/*const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}*/


void main()
{
	//vec2 uv = SampleSphericalMap(normalize(TexCoord)); // make sure to normalize localPos
    //Color.rgb = texture(Texture, uv).rgb * (dayNightRatio * 0.5 + 0.5);
	//Color.a = 1.0f;
	
	Color = texture(Texture, TexCoord) * (dayNightRatio * 0.5 + 0.5);
	//Color.a = 1.0f;
	
	float brightness = dot(Color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 0.1f)
		BrightnessColor = vec4(Color.rgb, 1.0f);
	else
		BrightnessColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}