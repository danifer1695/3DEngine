#version 330 core

out vec4 FragColor;

uniform vec3 selectColor;

void main(){
	FragColor = vec4(selectColor, 1.0);
}