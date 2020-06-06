#version 330 core

in vec2 texCoord;

uniform sampler2D matrixTexture;
uniform sampler2D ledOffTexture;

uniform int displayWidth;
uniform int displayHeight;
uniform int pointSize;
uniform vec3 emissiveColor;

layout (location = 0) out vec4 Color;
layout (location = 1) out vec4 Color2;

void main()
{
	float onOff = texture2D(matrixTexture, texCoord).r;
	
	// generate circle, radius = (pointSize - 2) / 2
	vec2 p = texCoord * vec2(displayWidth, displayHeight) * pointSize;
	
	vec2 c1 = vec2(int(p.x / pointSize), int(p.y / pointSize));
	vec2 center = c1 * pointSize + vec2(pointSize / 2.0f);
	
	float d = length(center - p);
	
	// emissive color
	Color.rgb = onOff * (1 - d / ((pointSize - 2) * 0.5f)) * emissiveColor;
	Color.a = 1.0f;
	
	// background
	Color2 = texture2D(ledOffTexture, texCoord *  vec2(displayWidth, displayHeight));
}
