#version 330 core

in vec2 texCoord;

uniform sampler2DMS Texture;

out vec4 Color;

float exposure = 0.05f;
//float exposure = 2.0f;

void main()
{
	vec2 temp = floor(textureSize(Texture) * texCoord);
	
	vec3 c = vec3(0, 0, 0);
	for (int i = 0; i < 8; ++i)
	{
		vec3 hdrColor = texelFetch(Texture, ivec2(temp), i).rgb;
		
		vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));
		//vec3 mapped = vec3(1.0f) - exp(-hdrColor * exposure);
		
		c += mapped;
	}
	
	
	float gamma = 2.2;
    Color.rgb = pow(c / 8, vec3(1.0/gamma));
	Color.a = 1.0f;
	
	
	
	//Color.rgb = vec3((Color.r + Color.g + Color.b) / 3.0f);
	//Color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}



/*
oryginalna wersja
#version 330 core

in vec2 texCoord;

uniform sampler2D Texture;

out vec4 Color;

float exposure = 0.05f;

void main()
{
	vec3 hdrColor = texture2D(Texture, texCoord).rgb;
	
	
	
	vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));
	//vec3 mapped = vec3(1.0f) - exp(-hdrColor * exposure);
	
	
	float gamma = 2.2;
    Color.rgb = pow(mapped, vec3(1.0/gamma));
	Color.a = 1.0f;
	
	
	
	//Color.rgb = vec3((Color.r + Color.g + Color.b) / 3.0f);
	//Color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}*/