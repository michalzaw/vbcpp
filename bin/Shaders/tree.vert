#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform vec3 CameraPositionWorldspace;
//uniform vec3 n;
uniform vec3 d;

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
	vec3 pos = VertexPosition;
	if (pos.y > 0.0f)
		pos += d;
	
	gl_Position = MVP * vec4(pos, 1.0f);

	Position = (ModelMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;//(ModelMatrix * vec4(VertexPosition, 1.0f)).xyz;
	TexCoord = VertexUV;
	//Normal = (NormalMatrix * vec4(normalize(vec3(VertexPosition.x, 0.0f, VertexPosition.z)), 0.0f)).xyz;//(NormalMatrix * vec4(VertexNormal, 0.0f)).xyz;//
	Normal = CameraPositionWorldspace - Position;
	Normal = normalize(vec3(Normal.x, 0.0f, Normal.z));
	//Normal = normalize(vec3(n.x, 0.0f, n.z));
	//Normal = n;
}