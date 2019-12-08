#version 330
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D ThisTexture;

void main()
{
	FragColor = texture(ThisTexture,TexCoords);
}