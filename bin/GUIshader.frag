#version 330 core

in vec3 Position;
in vec2 TexCoord;

out vec4 FragmentColor;

uniform sampler2D Texture;


void main()
{
	FragmentColor = texture2D(Texture, TexCoord);
}
