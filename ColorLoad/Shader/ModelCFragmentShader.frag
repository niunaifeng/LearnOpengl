#version 330 core
out vec4 FragColor;

in vec3 DiffColor;

void main()
{
	FragColor = vec4(DiffColor , 1.0);
}
