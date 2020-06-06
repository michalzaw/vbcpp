#version 330 core

layout (location = 0) in vec3 VertexPosition;
#ifdef ALPHA_TEST
layout (location = 1) in vec2 VertexUV;
#endif

uniform mat4 MVP;

#ifdef ALPHA_TEST
out vec2 uv;
#endif

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 1.0f);
	
#ifdef ALPHA_TEST
	uv = VertexUV;
#endif
}