#version 330 core

in vec2 texCoord;

uniform sampler2D matrixTexture;
uniform sampler2D ledOnTexture;
uniform sampler2D ledOffTexture;

uniform int displayWidth;
uniform int displayHeight;

out vec4 Color;

void main()
{
	float onOff = texture2D(matrixTexture, texCoord).r;
	
    Color.rgb = onOff * texture2D(ledOnTexture, texCoord * vec2(displayWidth, displayHeight)).rgb + (1 - onOff) * texture2D(ledOffTexture, texCoord *  vec2(displayWidth, displayHeight)).rgb;
    //Color.rgb = onOff * vec3(1.0f) + (1 - onOff) * vec3(0.0f);
    //Color.rgb = texture2D(ledOnTexture, vec2(texCoord.x * 116, texCoord.y * 16)).rgb;
	Color.a = 1.0f;
}