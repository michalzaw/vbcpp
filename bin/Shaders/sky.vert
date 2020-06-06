#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform vec3 CameraPosition;

out vec3 TexCoord;

void main()
{
	gl_Position = (MVP * vec4(VertexPosition + CameraPosition, 1.0f)).xyww;

	TexCoord = normalize(VertexPosition);
}