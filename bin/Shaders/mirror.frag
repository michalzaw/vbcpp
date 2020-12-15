#version 330 core


in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragmentColor;


uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform float Transparency;
uniform sampler2D Texture;
uniform float SpecularPower;


void main()
{
	FragmentColor = texture2D(Texture, vec2(TexCoord.x, 1.0f - TexCoord.y));
}
