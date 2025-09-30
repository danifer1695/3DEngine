#version 400 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D idTex;
uniform sampler2D renderTex;
uniform vec2 texelSize;

void main()
{	
	//vec3 idColor = texture(idTex, TexCoords).rgb;
	vec3 renderColor = texture(renderTex, TexCoords).rgb;

	vec3 result = renderColor;

	FragColor = vec4(result, 1.0);
}