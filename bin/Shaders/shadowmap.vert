#version 330 core

layout (location = 0) in vec3 VertexPosition;
#ifdef ALPHA_TEST
layout (location = 1) in vec2 VertexUV;
#endif
#ifdef ANIMATED
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 weights;
#endif

uniform mat4 MVP;

#ifdef ALPHA_TEST
out vec2 uv;
#endif
#ifdef ANIMATED
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
#endif

#ifdef ANIMATED
mat4 calculateVertexTransform()
{
	mat4 vertexTransform = mat4(0.0f);
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (boneIds[i] == -1)
			continue;

		if (boneIds[i] >= MAX_BONES)
		{
			vertexTransform = mat4(1.0f);
			break;
		}

		vertexTransform += finalBonesMatrices[boneIds[i]] * weights[i];
	}
	return vertexTransform;
}
#endif

void main()
{
	vec4 totalPosition = vec4(VertexPosition, 1.0f);
#ifdef ANIMATED
	mat4 vertexTransform = calculateVertexTransform();
	totalPosition = vertexTransform * vec4(VertexPosition, 1.0f);
#endif

	gl_Position = MVP * totalPosition;
	
#ifdef ALPHA_TEST
	uv = VertexUV;
#endif
}