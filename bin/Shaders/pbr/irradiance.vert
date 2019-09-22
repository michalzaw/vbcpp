#version 330 core

layout (location = 0) in vec3 VertexPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 TexCoord;

void main()
{
	gl_Position = (projectionMatrix * viewMatrix * vec4(VertexPosition, 1.0f)).xyww;

	TexCoord = normalize(VertexPosition);
}