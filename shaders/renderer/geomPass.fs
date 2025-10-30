#version 330 core
//we export this data to the g-buffer's multiple color attachments we set up
layout (location = 0) out vec4 gNormal;		//RGB - Normal, A - Glossiness
layout (location = 1) out vec4 gAlbedoSpec;	//RGB - Albedo, A - Specular
layout (location = 2) out vec4 gEmissive;	//RGB - Emissive, A - 

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 Tangent;

//This is ran once per item so assuming each item only has one map of each type, we just
//need one variable for each
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D glossinessMap;
uniform sampler2D emissiveMap;

//non-texture colors
uniform vec3	diffuseColor;
uniform float	specularColor;
uniform vec3	normalColor;
uniform vec3	emissiveColor;
uniform float	glossinessColor;

//texture map check
uniform bool useDiffuseMap;
uniform bool useSpecularMap;
uniform bool useNormalMap;
uniform bool useGlossinessMap;
uniform bool useEmissiveMap;

void main()
{
	//we export all relevant data to their corresponding color attachment

	//Has Normal map?
	if(useNormalMap) 
	{
		vec3 normalTex = texture(normalMap, TexCoords).rgb;
		normalTex = normalize(normalTex * 2.0 + 1.0);	//convert [0, 1] to [-1, 1]

		vec3 T = normalize(Tangent);
		vec3 N = normalize(Normal);
		vec3 B = normalize(cross(N, T));

		mat3 TBN = mat3(T, B, N);
		vec3 finalNormal = TBN * normalTex;

		gNormal.rgb	= normalize(finalNormal) * 0.5 + 0.5;
	}
	else
	{
		gNormal.rgb = normalize(Normal) * 0.5 + 0.5;
	}

	//Has Glossiness Map?
	useGlossinessMap ? gNormal.a = texture(glossinessMap, TexCoords).r : gNormal.a = glossinessColor;

	//Has Emissive Map?
	useEmissiveMap ? gEmissive.rgb = texture(emissiveMap, TexCoords).rgb : gEmissive.rgb = emissiveColor;

	//Has Diffuse Map?
	useDiffuseMap ? gAlbedoSpec.rgb = texture(diffuseMap, TexCoords).rgb : gAlbedoSpec.rgb = diffuseColor;

	//Has Specular Map?
	useSpecularMap ? gAlbedoSpec.a = texture(specularMap, TexCoords).r : gAlbedoSpec.a = specularColor;
}