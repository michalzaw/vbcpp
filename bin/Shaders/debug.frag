#version 330 core

out vec4 FragmentColor;

uniform vec4 matEmissive;

void main()
{
	FragmentColor = matEmissive;
}
