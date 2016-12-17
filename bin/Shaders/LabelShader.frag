#version 330 core

in vec3 Position;
in vec2 TexCoord;

out vec4 FragmentColor;

uniform vec4 color;
uniform sampler2D Texture;


void main()
{
	vec4 c = texture2D(Texture, TexCoord);
	FragmentColor = vec4(c.r, c.r, c.r, c.r) * color;
}
