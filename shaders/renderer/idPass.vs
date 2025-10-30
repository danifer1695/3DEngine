#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 viewPos = view * model * vec4(aPos, 1.0);
    FragPos = viewPos.xyz; 

	TexCoords = aTexCoords;

	gl_Position = projection * viewPos;
}