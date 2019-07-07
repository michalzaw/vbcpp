#version 330 core

layout (location = 0) in vec2 VertexPosition;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(VertexPosition.x, VertexPosition.y, 0.5f, 1.0f);
	
	texCoord = VertexPosition * 0.5 + vec2(0.5, 0.5);
}