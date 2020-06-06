#version 330 core

const int SAMPLES_COUNT = {samplesCount};

out vec4 FragmentColor;
  
in vec2 texCoord;

#ifdef MULTISAMPLE
uniform sampler2DMS Texture;
#endif
#ifdef NOT_MULTISAMPLE
uniform sampler2D Texture;
#endif

  
uniform bool isHorizontal;
float weight[10] = float[] (0.016815,0.023597,0.031821,0.041234,0.051343,0.061432,0.07063,0.078033,0.082841,0.084509);



#ifdef NOT_MULTISAMPLE
void main()
{
    vec2 tex_offset = 1.0 / textureSize(Texture, 0);
    vec3 result = texture(Texture, texCoord).rgb * weight[0];
	
	if(isHorizontal)
    {
        for(int i = 1; i < 10; ++i)
        {
            result += texture(Texture, texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(Texture, texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 10; ++i)
        {
            result += texture(Texture, texCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(Texture, texCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragmentColor = vec4(result * 0.7, 1.0);
}
#endif

#ifdef MULTISAMPLE
void main()
{
	vec2 temp = floor(textureSize(Texture) * texCoord);
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
	
	for (int j = 0; j < SAMPLES_COUNT; ++j)
	{
		result += texelFetch(Texture, ivec2(temp), 0).rgb * weight[0];
		result += texelFetch(Texture, ivec2(temp), 0).rgb * weight[0];
	}
	
    if(isHorizontal)
    {
        for(int i = 1; i < 10; ++i)
        {
			for (int j = 0; j < SAMPLES_COUNT; ++j)
			{
				result += texelFetch(Texture, ivec2(temp + vec2(i, 0.0)), j).rgb * weight[i];
				result += texelFetch(Texture, ivec2(temp - vec2(i, 0.0)), j).rgb * weight[i];
			}
        }
    }
    else
    {
        for(int i = 1; i < 10; ++i)
        {
			for (int j = 0; j < SAMPLES_COUNT; ++j)
			{
				result += texelFetch(Texture, ivec2(temp + vec2(0.0, i)), j).rgb * weight[i];
				result += texelFetch(Texture, ivec2(temp - vec2(0.0, i)), j).rgb * weight[i];
			}
        }
    }
			
	result = result / SAMPLES_COUNT;
	
    FragmentColor = vec4(result * 0.7, 1.0);
}