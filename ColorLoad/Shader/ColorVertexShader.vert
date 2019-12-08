#version 330
layout (location = 0) in vec3 aPos;

layout (location = 2) in vec3 aColor;
out vec3 Color;
void main()
{
	gl_Position = vec4(aPos + vec3(0.5f,0.0f,0.0f), 1.0);
	Color = aColor;
}