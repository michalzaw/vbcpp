#version 330 core

in vec3 TexCoord;

//uniform samplerCube Texture;
uniform sampler2D Texture;
uniform float dayNightRatio;

out vec4 Color;


const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}


void main()
{
	vec2 uv = SampleSphericalMap(normalize(TexCoord)); // make sure to normalize localPos
    Color.rgb = texture(Texture, uv).rgb;
	Color.a = 1.0f;
	
	//Color = texture(Texture, TexCoord) * (dayNightRatio * 0.5 + 0.5);
}