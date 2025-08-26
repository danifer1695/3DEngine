#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTex;
uniform bool toggleEffects;

vec3 chromaticAberration()
{
	vec3 result = vec3(0.0);

	vec2 offset = 0.01 * (TexCoords - 0.5);	//color offset;
	result.r = texture(screenTex, TexCoords + offset).r;
	result.g = texture(screenTex, TexCoords).g;
	result.b = texture(screenTex, TexCoords - offset).b;

	return result;
}

vec3 addVignette(vec3 color)
{
	vec3 result = color;

	float dist = distance(TexCoords, vec2(0.5));	//fragment distance to the center of the screen
	float innerLimit = 0.5;
	float outerLimit = 1.0;
	float vignette = smoothstep(outerLimit, innerLimit, dist);
	result *= vignette;

	return result;
}

void main()
{
	vec3 color = vec3(0.0f);

	if(!toggleEffects) color = vec3(texture(screenTex, TexCoords));
	else
	{
		color = texture(screenTex, TexCoords).rgb;
		//color = chromaticAberration();
		color = addVignette(color);
	}

	FragColor = vec4(color, 1.0);
}