#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
void main()
{
	gl_Position = vec4(aPos + vec3(-0.5f,0.0f,0.0f), 1.0);
	TexCoords = aTexCoords;
}