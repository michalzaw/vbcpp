#version 330 core

layout (location = 0) in vec3 VertexPosition;

uniform mat4 VerticesTransformMatrix;
uniform mat4 TexCoordTransformMatrix;

out vec2 TexCoord;

void main()
{
	gl_Position = VerticesTransformMatrix * vec4(VertexPosition, 1.0f);

	TexCoord = (TexCoordTransformMatrix * vec4(VertexPosition, 1.0f)).xy;
	//TexCoord = VertexPosition.xy;
}