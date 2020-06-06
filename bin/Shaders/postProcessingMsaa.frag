#version 330 core

const int SAMPLES_COUNT = {samplesCount};

in vec2 texCoord;

uniform sampler2DMS texture1;

out vec4 Color;


void main()
{
	vec2 temp = floor(textureSize(texture1) * texCoord);
	
	vec3 resultColor = vec3(0, 0, 0);
	for (int i = 0; i < SAMPLES_COUNT; ++i)
	{
		vec3 hdrColor = texelFetch(texture1, ivec2(temp), i).rgb;
		
		resultColor += hdrColor;
	}
	
	Color.rgb = resultColor / SAMPLES_COUNT;
	Color.a = 1.0f;
}