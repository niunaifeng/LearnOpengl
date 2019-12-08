#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 5) in vec3 aDiff;

out vec3 DiffColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	DiffColor = aDiff;
	//DiffColor = vec3(1.0f,1.0f,1.0f);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}