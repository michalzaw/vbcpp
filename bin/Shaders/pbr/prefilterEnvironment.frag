#version 330 core

in vec3 TexCoord;

uniform samplerCube Texture;
uniform float roughness;

out vec4 color;


const float PI = 3.14159265359;


float DistributionGGX(vec3 N, vec3 H, float a)
{
	float a2     = a*a * a * a; // było a * a
	float NdotH  = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom    = a2;
	float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
	denom        = PI * denom * denom;

	return nom / denom;
}


float radicalInverseVdc(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley(uint i, uint n)
{
	return vec2(float(i) / float(n), radicalInverseVdc(i));
}


vec3 importanceSampleGGX(vec2 xi, vec3 normal, float roughness)
{
	float a = roughness * roughness;
	
	float phi = 2.0f * PI * xi.x;
	float cosTheta = sqrt((1.0f - xi.y) / (1.0f + (a * a - 1.0f) * xi.y));
	float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
	
	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	
	vec3 up = abs(normal.z) < 0.999 ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 tangent = normalize(cross(up, normal));
	vec3 bitangent = cross(normal, tangent);
	
	vec3 sampleVec = tangent * H.x + bitangent * H.y + normal * H.z;
	return normalize(sampleVec);
}


void main()
{
	vec3 normal = normalize(TexCoord);
	vec3 r = normal;
	vec3 v = r;
	
	const uint SAMPLE_COUNT = 4096u;
	float totalWeight = 0.0f;
	vec3 prefilteredColor = vec3(0.0f);
	
	for (uint i = 0u; i < SAMPLE_COUNT; ++i)
	{
		vec2 xi = hammersley(i, SAMPLE_COUNT);
		vec3 h = importanceSampleGGX(xi, normal, roughness);
		vec3 l = normalize(2.0f * dot(v, h) * h - v);
		
		float ndotl = max(dot(normal, l), 0.0f);
		if (ndotl > 0.0f)
		{
			float d = DistributionGGX(normal, h, roughness);
			float NdotH = max(dot(normal, h), 0.0f);
			float HdotV = max(dot(h,v), 0.0f);
			float pdf = d * NdotH / (4 * HdotV) + 0.0001;
			
			float RESOLUTION = 768.0f;
			float texel = 4.0f * PI / (6.0f * RESOLUTION * RESOLUTION);
			float sample = 1.0f / (float(SAMPLE_COUNT) * pdf + 0.0001);
			
			float mipLevel = roughness == 0.0f ? 0.0f : 0.5f * log2(sample / texel);
			
			prefilteredColor += textureLod(Texture, l, mipLevel).rgb * ndotl;
			totalWeight += ndotl;
		}
	}
	
	prefilteredColor = prefilteredColor / totalWeight;
	
	
	color = vec4(prefilteredColor, 1.0f);
}