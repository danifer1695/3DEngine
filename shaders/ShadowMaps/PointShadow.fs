#version 400 core

in vec4 FragPos;
in vec3 FragPosWorld;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	float dist = length(FragPosWorld - lightPos);
	//map to [0, 1] by dividing by farPlane
	dist = dist / farPlane;
	//we store the depth info into the framebuffer's depth attachment
	gl_FragDepth = dist;

}