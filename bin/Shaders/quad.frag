#version 330 core

const int SAMPLES_COUNT = {samplesCount};

in vec2 texCoord;

#ifdef MULTISAMPLE
uniform sampler2DMS Texture;
#endif
#ifdef NOT_MULTISAMPLE
uniform sampler2D Texture;
#endif

out vec4 Color;

float exposure = 0.05f;
//float exposure = 2.0f;


vec3 toneMap(vec3 hdrColor)
{
	return hdrColor / (hdrColor + vec3(1.0f));
	
	//return vec3(1.0f) - exp(-hdrColor * exposure);
}


void main()
{
#ifdef MULTISAMPLE
	vec2 temp = floor(textureSize(Texture) * texCoord);
	
	vec3 resultColor = vec3(0, 0, 0);
	for (int i = 0; i < SAMPLES_COUNT; ++i)
	{
		vec3 hdrColor = texelFetch(Texture, ivec2(temp), i).rgb;

		vec3 ldrColor = toneMap(hdrColor);
		
		resultColor += ldrColor;
	}
	
	resultColor = resultColor / SAMPLES_COUNT;
#endif

#ifdef NOT_MULTISAMPLE
	vec3 hdrColor = texture2D(Texture, texCoord).rgb;

	vec3 resultColor = toneMap(hdrColor);
#endif
	
	// gamma correction;
	float gamma = 2.2;
    Color.rgb = pow(resultColor, vec3(1.0 / gamma));
	Color.a = 1.0f;
}