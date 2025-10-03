#version 330 core
//we export this data to the g-buffer's multiple color attachments we set up
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;	//RGB - Albedo, A - Specular

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 Tangent;

//This is ran once per item so assuming each item only has one map of each type, we just
//need one variable for each
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform bool useNormalMap;

void main()
{
	//we export all relevant data to their corresponding color attachment
	gPosition = FragPos;

	if(useNormalMap) 
	{
		vec3 normalTex = texture(normalMap, TexCoords).rgb;
		normalTex = normalize(normalTex * 2.0 - 1.0);	//convert [0, 1] to [-1, 1]

		vec3 T = normalize(Tangent);
		vec3 N = normalize(Normal);
		vec3 B = normalize(cross(N, T));

		mat3 TBN = mat3(T, B, N);
		vec3 finalNormal = TBN * normalTex;

		gNormal		= normalize(finalNormal);
	}
	else
	{
		gNormal = normalize(Normal);
	}
	gAlbedoSpec.rgb = texture(diffuseMap, TexCoords).rgb;
	gAlbedoSpec.a = texture(specularMap, TexCoords).r;
}