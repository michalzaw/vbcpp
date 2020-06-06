#version 330 core

in vec3 TexCoord;

uniform samplerCube Texture;

out vec4 color;


const float PI = 3.14159265359;


void main()
{
	vec3 normal = normalize(TexCoord);
	
	vec3 irradiance = vec3(0.0f);
	
	
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = cross(up, normal);
	up = cross(normal, right);
	
	float sampleDelta = 0.025;
	float samplesCount = 0.0f;
	for (float phi = 0.0f; phi < 2.0f * PI; phi += sampleDelta)
	{
		for (float theta = 0.0f; theta < 0.5f * PI; theta += sampleDelta)
		{
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
			
			irradiance += texture(Texture, sampleVec).rgb * cos(theta) * sin(theta);
			
			++samplesCount;
		}
	}
	
	irradiance = PI * irradiance * (1.0f / float(samplesCount));
	
	
	color = vec4(irradiance, 1.0f);
}