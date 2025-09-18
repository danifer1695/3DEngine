#version 400 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 idColor;

void main()
{
	FragColor = vec4(idColor, 1.0);
}