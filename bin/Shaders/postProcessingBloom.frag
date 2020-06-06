#version 330 core

in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 Color;

//uniform float BloomRatio;


void main()
{
	float BloomRatio = 1.0f;
	Color.rgb = texture2D(texture1, texCoord).rgb + BloomRatio * texture2D(texture2, texCoord).rgb;
	Color.a = 1.0f;
}