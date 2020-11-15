#version 330 core

out vec4 fragmentColor;

in vec3 color;

void main()
{
	fragmentColor = vec4(color, 1.0f);
}
