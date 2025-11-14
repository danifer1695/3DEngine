#version 400 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D iconTexture;
uniform vec3 lightColor;

void main()
{
	vec4 texColor = texture(iconTexture, TexCoords);

	//Skip transparent fragments
	if(texColor.a < 0.1)
		discard;

	//Key color
	vec3 keyColor = vec3(1.0, 1.0, 1.0);
	float toleranceInner = 0.4;
	float toleranceOuter = 1.0;

	//Texture distance from keyColor
	float diff = length(texColor.rgb - keyColor);

	//blend factor -> 0 = original, 1 = replaced
	float t = 1.0 - smoothstep(toleranceInner, toleranceOuter, diff);
	
	//blend original and light color
	vec3 blended = mix(texColor.rgb, lightColor, t);

	FragColor = vec4(blended, texColor.a);
}