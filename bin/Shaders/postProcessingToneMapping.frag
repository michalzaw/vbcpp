#version 330 core

in vec2 texCoord;

uniform sampler2D texture1;

out vec4 Color;

uniform float exposure;
uniform float visibility;


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
    float e = 0.14;//0.370013;
	//0.336375
	//e = exposure;
	//e = 0.336375;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0, 1);
}
#endif

#ifdef DEPTH_RENDERING
vec3 toneMap(vec3 hdrColor)
{
	//float ndcDepth = (2.0 * hdrColor.r - 0.1f - 10.0f) / (10.0f - 0.1f);
	//return vec3((ndcDepth * 0.5) + 0.5);

	float n = 0.1; // camera z near
	float f = 1000.0; // camera z far
	float z = hdrColor.r;
	return vec3((2.0 * n) / (f + n - z * (f - n)));
}
#endif

#ifdef NONE
vec3 toneMap(vec3 hdrColor)
{
	return hdrColor;	
}
#endif


void main()
{
	vec3 hdrColor = texture2D(texture1, texCoord).rgb;

	vec3 resultColor = toneMap(hdrColor);
	
	// gamma correction;
	float gamma = 2.2;
	Color.rgb = pow(resultColor, vec3(1.0 / gamma)) * visibility;
	Color.a = 1.0f;
}