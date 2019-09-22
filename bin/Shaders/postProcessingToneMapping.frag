#version 330 core

in vec2 texCoord;

uniform sampler2D texture1;

out vec4 Color;

uniform float exposure;


#ifdef REINHARD
vec3 toneMap(vec3 hdrColor)
{
	return hdrColor / (hdrColor + vec3(1.0f));
}
#endif

#ifdef CLASSIC
vec3 toneMap(vec3 hdrColor)
{
	return vec3(1.0f) - exp(-hdrColor * exposure);
}
#endif

#ifdef ACES
vec3 toneMap(vec3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.370013;//0.14f;
	//0.336375
	//e = exposure;
	//e = 0.336375;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0, 1);
}
#endif


void main()
{
	vec3 hdrColor = texture2D(texture1, texCoord).rgb;

	vec3 resultColor = toneMap(hdrColor);
	
	// gamma correction;
	float gamma = 2.2;
    Color.rgb = pow(resultColor, vec3(1.0 / gamma));
	Color.a = 1.0f;
}