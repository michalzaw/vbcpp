#version 330 core

uniform vec4 matDiffuse;

//out vec4 FragmentColor;
layout (location = 0) out vec4 FragmentColor;
layout (location = 1) out vec4 BrightnessColor;
#ifdef RENDER_OBJECT_ID
layout (location = 2) out uvec4 ObjectIdValue;
#endif

void main()
{
	FragmentColor = matDiffuse;
}
