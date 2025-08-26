#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	float dist = length(FragPos.xyz - lightPos);
	//map to [0, 1] by dividing by farPlane
	dist = dist / farPlane;
	//we store the depth info into the framebuffer's depth attachment
	gl_FragDepth = dist;

}