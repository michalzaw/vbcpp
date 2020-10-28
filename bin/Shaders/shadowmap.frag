#version 330 core

#ifdef ALPHA_TEST
uniform sampler2D AlphaTexture;

in vec2 uv;
#endif

out vec4 FragmentColor;

void main()
{
#ifdef ALPHA_TEST
	if (texture2D(AlphaTexture, uv).a < 0.1f) // dla pbr_tree powinno byc 0.4
		discard;
#endif

	FragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
