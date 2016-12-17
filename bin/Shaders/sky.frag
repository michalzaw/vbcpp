#version 330 core

in vec3 TexCoord;

uniform samplerCube Texture;

out vec3 Color;


void main()
{
	Color = texture(Texture, TexCoord).rgb;
}