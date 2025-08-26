#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 worldPos;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform vec3 viewPos;

void main()
{
	//Ambient
	vec3 color = texture(diffuseMap, TexCoords).rgb;

	FragColor = vec4(color, 1.0);
}