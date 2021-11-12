#version 330 core

in vec2 TexCoord;

out vec4 FragmentColor;

uniform vec4 color;
uniform float progressBarValue;
uniform float progressBarType;


#define M_PI 3.1415926535897932384626433832795


void main()
{
	// ------------------------------

	vec2 uv = TexCoord * 2.0f - 1.0f;
	
	// ------------------------------

	float radius_2 = uv.x * uv.x + uv.y * uv.y;
	float isCircle;
	if (radius_2 > 0.25f && radius_2 < 0.5)
		isCircle = 1.0f;
	else
		isCircle = 0.0f;

	// ------------------------------

	float angle = atan(uv.x, -uv.y);
	// remap to range [0; 1]
	angle = (angle + M_PI) / (2 * M_PI);

	float progressBarValue2 = progressBarValue;

	float s = 1;
	for (int i = 0; i < floor(progressBarValue2); ++i)
	{
		s *= -1;
	}

	progressBarValue2 = progressBarValue2 - floor(progressBarValue2);
	// remap to range [-1; 1]
	progressBarValue2 = progressBarValue2 * 2 - 1;


	//Color.rgb = vec3((angle + progressBarValue) * isCircle) * vec3(0.0f, 0.8f, 0.1f);
	
	// -----------------------------------------
	// 1
	float newAngle = angle;
	if (s < 0.0f)
	{
		newAngle = 1 - angle;
	}

	float value1 = clamp(newAngle + s *progressBarValue2, 0, 1) * isCircle;

	// -----------------------------------------
	// 2
	float value2 = clamp((angle + progressBarValue) - floor(angle + progressBarValue), 0, 1) * isCircle;
	
	// -----------------------------------------

	FragmentColor = vec4((1.0f - progressBarType) * value1 + progressBarType * value2) * color;

    //vec2 dist =	vec2(1.0, 0.0) - vec2(-1.0, 0.0);
	//float val = dot(uv - vec2(-1, 0), dist) / dot(dist, dist);
	//val = clamp(val, 0, 1);

	//vec3 color = mix(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), val);

	//Color.rgb = color;
	//Color.a = isCircle;
}