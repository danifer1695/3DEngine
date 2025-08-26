#version 330 core

out vec4 FragColor;

in vec3 WorldPos;

uniform samplerCube cubeMap;
uniform int skybox_type;

void main()
{
	vec3 envColor = vec3(0.0);

	//==========================================================================================
	//if skybox is of type SKY_IMAGE, sample cubemap texture
	if(skybox_type == 0)
	{
		envColor = texture(cubeMap, WorldPos).rgb;

		//HDR tonemap and gamma correction done in screenShader
		float exposure = 1.5;
		envColor = vec3(1.0) - exp(-envColor * exposure);
		envColor = pow(envColor, vec3(1.0/2.2));
	}

	//==========================================================================================
	//if skybox is of type SKY_GRADIENT, return interpolated value based on UV's y coords
	if(skybox_type == 1)
	{
		float domeValue = min(smoothstep(-1.0, 1.0, WorldPos.y) + 0.3, 0.8);
		envColor = vec3(domeValue);
	}

	//==========================================================================================
	
	FragColor = vec4(envColor, 1.0);
}