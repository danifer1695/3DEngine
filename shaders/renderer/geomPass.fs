#version 330 core
//we export this data to the g-buffer's multiple color attachments we set up
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;	//RGB - Albedo, A - Specular

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

//This is ran once per item so assuming each item only has one map of each type, we just
//need one variable for each
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

void main()
{
	//we export all relevant data to their corresponding color attachment
	gPosition = FragPos;
	//gPosition = vec3(1.0, 0.0, 1.0);	//debugging
	gNormal = normalize(Normal);
	gAlbedoSpec.rgb = texture(diffuseMap, TexCoords).rgb;
	//gAlbedoSpec.rgb = vec3(1.0, 1.0, 0.0).rgb;
	gAlbedoSpec.a = texture(specularMap, TexCoords).r;
}