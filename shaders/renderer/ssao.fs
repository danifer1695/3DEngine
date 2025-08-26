#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;		//4x4 noise texture generated in the SSAO class

uniform vec3 samples[64];		//samples per kernel (max size 64)
uniform int sampleNr;			//size of the 'samples' array

float radius = 0.5;				//kernel sampling radius
float bias = 0.025;

uniform float screenWidth;		//check: set
uniform float screenHeight;		//check: set
vec2 noiseScale;

uniform mat4 projection;

void main()
{
	//setup noiseScale vector
	noiseScale = vec2(screenWidth/4.0, screenHeight/4.0);	//noise texture is 4x4 in size

	//get gbuffer data
	vec3 fragPos =	texture(gPosition, TexCoords).xyz;	//gbuffer sends infor in view

	vec3 normal =	normalize(texture(gNormal, TexCoords).rgb);

	vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

	//tangent space to view space matrix (for the kernel info)
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	//iterate over the kernel vectors to calculate occlusion factor
	float occlusion = 0.0;

	int count = min(sampleNr, 64);		//make sure we dont sample beyond max size of the sample array
	for(int i = 0; i < count; ++i)
	{
		//get sample position
		vec3 samplePos = TBN * samples[i];	//transform from tangent to view space
		samplePos = fragPos + samplePos * radius;

		//if  sample is beyond the hemisphere's radius, continue to next iteration
		if (abs(samplePos.z - fragPos.z) > radius) continue;

		//Project into screen space for texture lookup;
		vec4 offset = projection * vec4(samplePos, 1.0);	//multiply by project and view matrix to transfrom coords to clip space (origin = center of frustrum)
		offset.xyz /= offset.w;					//perspective divide - coordinates are now in clip space [-1.0, 1.0]
		offset.xyz = offset.xyz * 0.5 + 0.5;	//Map from clip space [-1,1] to texture coordinates [0,1].

		//sample depth
		float sampleDepth = texture(gPosition, offset.xy).z;		//world space coordinates of the sampled offset fragment

		//chech range of samples so that fragments that are too far behind dont affect AO
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		//if the sampled depth value is larger than the fragment's we add 1 to the occlusion value.
        //if the conditional statement returns 1, its value will be interpolated based on its distance
        //(further away depths will have their impact minimized)
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	//average results
	occlusion = 1.0 - (occlusion / float(count));

	FragColor = occlusion;
}