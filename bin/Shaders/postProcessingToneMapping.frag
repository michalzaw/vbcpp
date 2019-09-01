#version 330 core

in vec2 texCoord;

uniform sampler2D texture1;

out vec4 Color;

uniform float exposure;

vec3 toneMap(vec3 hdrColor)
{
	//return hdrColor / (hdrColor + vec3(1.0f));
	
	return vec3(1.0f) - exp(-hdrColor * exposure);
}


void main()
{
	vec3 hdrColor = texture2D(texture1, texCoord).rgb;

	vec3 resultColor = toneMap(hdrColor);
	
	// gamma correction;
	float gamma = 2.2;
    Color.rgb = pow(resultColor, vec3(1.0 / gamma));
	Color.a = 1.0f;
}