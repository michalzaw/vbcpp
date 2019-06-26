#version 330 core

layout (location = 0) in float vertexIndex;

uniform vec3 indices[8];

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(indices[int(vertexIndex)].x, indices[int(vertexIndex)].y, indices[int(vertexIndex)].z, 1.0f);
}