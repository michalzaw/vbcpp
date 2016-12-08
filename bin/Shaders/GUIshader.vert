#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 UV;

uniform mat4 VerticesTransformMatrix;
uniform mat4 TexCoordTransformMatrix;

out vec2 TexCoord;

void main()
{
	gl_Position = VerticesTransformMatrix * vec4(VertexPosition, 1.0f);

	//TexCoord = UV;
	TexCoord = (TexCoordTransformMatrix * vec4(UV, 0.0f, 1.0f)).xy;
	//TexCoord = VertexPosition.xy;
}