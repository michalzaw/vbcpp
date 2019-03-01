#version 330 core

in vec3 TexCoord;

uniform samplerCube Texture;
uniform float dayNightRatio;

out vec4 Color;


void main()
{
	Color = texture(Texture, TexCoord) * (dayNightRatio * 0.5 + 0.5);
}