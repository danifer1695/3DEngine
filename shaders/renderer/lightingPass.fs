#version 400 core
out vec4 FragColor;

in vec2 TexCoords;

//We take in the information output by the gbuffer (geometry pass)
uniform sampler2D gDepth;						//check: set
uniform sampler2D gNormal;						//check: set
uniform sampler2D gAlbedoSpec;					//check: set
uniform sampler2D AOMap;						//check: set

//Environment
uniform bool irradianceActive;					//check: set
uniform bool ssaoEnabled;						//check: set
uniform samplerCube irradianceMap;				//check: set

//Camera & Material
uniform float materialShininess;				//check: set
uniform float farPlane;							//check: set
uniform mat4 viewMatrix;						//check: set
uniform mat4 inverseViewMatrix;					//check: set
uniform mat4 inverseProjMatrix;					//check: set
	
//Light info
struct PointLight
{
	vec3 Position;								//check: set
	bool isActive;								//check: set

	vec3 Color;									//check: set
	float Intensity;							//check: set
	
	float Radius;								//check: set
	bool CastShadow;							//check: set
	bool SoftShadow;							//check: set
	bool dummy;									//dummy space to keep groups of 4
};

struct DirectionalLight
{
	vec3 Position;								//check: set
	bool isActive;								//check: set

	vec3 Color;									//check: set
	float Intensity;							//check: set
	
	vec3 Direction;								//check: set
	bool CastShadow;							//check: set

	vec3 dummy;
	bool SoftShadow;							//check: set

	mat4 LightSpaceMatrix;						//check: set
};

const int MAX_POINT_LIGHTS = 8;
const int MAX_DIR_LIGHTS = 8;
const int MAX_SPOT_LIGHTS = 4;

uniform int numberOfPointLights;								//check: set
uniform int numberOfDirLights;									//check: set
uniform int numberOfSpotLights;									//check: set

uniform PointLight pointLights[MAX_POINT_LIGHTS];				//check: set
uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];				//check: set

uniform sampler2DArray dirShadowArray;							//check: set
uniform samplerCubeArray pointShadowArray;						//check: set

vec3 toLinear(vec3 c) { return pow(c, vec3(2.2)); }
vec3 toSRGB(vec3 c)   { return pow(c, vec3(1.0/2.2)); }

//array of offset dierctions around the fragment where we will sample
const int MAX_SAMPLES = 20;
vec3 sampleOffsets[MAX_SAMPLES] = vec3[]
(
	vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 randomUnitVector(vec3 seed)
{
	//generate a pseudo-random unit vector, using the fragment's world position as a seed
	float angle1 = fract(sin(dot(seed, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
    float angle2 = fract(sin(dot(seed, vec3(39.3467, 11.135, 94.673))) * 13214.3458);
    float theta = angle1 * 6.28318530718; // [0, 2pi]
    float phi = angle2 * 3.14159265359;   // [0, pi]
    return vec3(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
}

mat3 buildRotationMatrix(vec3 dir)
{
	vec3 up = abs(dir.y) < 0.99 ? vec3(0, 1, 0) : vec3(1, 0, 0);
	vec3 tangent = normalize(cross(up, dir));
	vec3 bitangent = cross(dir, tangent);
	return mat3(tangent, bitangent, dir);
}

int DynamicSampling(vec3 FragPos)
{
	float FragDist = -FragPos.z;
	
	//sample number bracket
	int maxSamples = 12;
	int minSamples = 4;

	//distance bracket
	float maxDistance = 50.0;
	float minDistance = 0.5;

	float t = clamp((FragDist - minDistance) / (maxDistance - minDistance), 0.0, 1.0);
	int sampleCount = int(mix(maxSamples, minSamples, t));

	return sampleCount;
}

vec3 reconstructPosition(float depth)
{
	//NDC coordinates
	vec3 ndc;
	ndc.xy = TexCoords * 2.0 - 1.0;
	ndc.z = depth * 2.0 - 1.0;

	//Return to view space
	vec4 clipPos = vec4(ndc, 1.0);
	vec4 viewPos = inverseProjMatrix * clipPos;
	viewPos /= viewPos.w;

	return viewPos.xyz;
}

float PointShadowCalculation(PointLight light, int lightIndex, vec3 FragPos, vec3 fragPosWorld)
{
	vec3 lightPosWorld = (inverseViewMatrix * vec4(light.Position, 1.0)).xyz;

	vec3 lightToFrag = fragPosWorld - lightPosWorld;	//both should be in view space
	float depth = length(lightToFrag);
	lightToFrag = normalize(lightToFrag);			//we will use this to sample the cubemap
	//vec3 samplingVector = (inverse(viewMatrix) * vec4(lightToFrag, 1.0)).xyz;

	float shadow = 0.0;
	float bias = 0.15;
	float lightDistance = depth;
	float diskRadius = (1.0 + (lightDistance / farPlane)) / 105.0;

	if(light.SoftShadow)
	{
		int samples = DynamicSampling(FragPos);
		//Random rotation matrix
		vec3 randVec = randomUnitVector(fragPosWorld);
		mat3 rot = buildRotationMatrix(randVec);
	
		for(int i = 0; i < samples; ++i)
		{
			vec3 offset = rot * sampleOffsets[i];
			vec3 dir = normalize(lightToFrag + offset * diskRadius);
			float sampledDepth = texture(pointShadowArray, vec4(dir, lightIndex)).r;
			sampledDepth *= farPlane;

			if(depth - bias > sampledDepth)
				shadow += 1.0;
		}
		shadow /= float(samples);
	}
	else
	{
		float sampledDepth = texture(pointShadowArray, vec4(lightToFrag, lightIndex)).r;
		sampledDepth *= farPlane;
		if(depth - bias > sampledDepth) shadow = 1.0;
	}

	return 1.0 - shadow;
	//return texture(light.ShadowMap, lightToFrag).r;
	//return depth / 30.0;
}

float DirShadowCalculation(DirectionalLight light, int lightIndex, vec3 FragPos, vec3 fragPosWorld, vec3 Normal)
{
	vec4 fragPosLight = light.LightSpaceMatrix * vec4(fragPosWorld, 1.0);

	vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
	projCoords = projCoords * 0.5 + 0.5;

	//End calculations early if fragment is outside frustrum
	if(projCoords.x < 0.0 || projCoords.x > 1.0 ||
		projCoords.y < 0.0 || projCoords.y > 1.0 ||
		projCoords.z < 0.0 || projCoords.z > 1.0)
		return 1.0;

	float closestDepth = texture(dirShadowArray[lightIndex], vec3(projCoords.xy, lightIndex)).r;
	float currentDepth = projCoords.z;

	vec3 normal = normalize(Normal);
	vec3 lightDir = light.Direction;
	float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0025);

	float shadow = 0.0;

	if(light.SoftShadow)
	{
		int samples = DynamicSampling(FragPos);
		float softness = 1.0;
		float diskRadius = (1.0 + softness) / float(textureSize(dirShadowArray[lightIndex], 0).x);

		//Random rotation matrix
		vec3 randVec = randomUnitVector(FragPos);
		mat3 rot = buildRotationMatrix(randVec);

		for(int i = 0; i < samples; ++i)
		{
			vec2 offset = vec2(rot * sampleOffsets[i]);
			vec2 sampleXY = projCoords.xy + offset * diskRadius;
			float sampledDepth = texture(dirShadowArray[lightIndex], vec3(sampleXY, lightIndex)).r;

			if(currentDepth - bias > sampledDepth)
				shadow += 1.0;
		}
		shadow /= samples;
	}
	else
	{
		if(currentDepth - bias > closestDepth)
			shadow = 1.0;
	}

	return 1 - shadow;
	
}

float GetAttenuation(float dist, float radius)
{
	//avoid dividing by zero
	float d = max(dist, 0.001);

	if(d >= radius) return 0.0;

	float x = d / radius;

	//smooth fade term
	float smoothFactor = pow(1.0 - pow(x, 4.0), 2.0);

	return smoothFactor / (d * d);
}

vec3 Lighting_PointLight(PointLight light, int lightIndex, vec3 FragPos, vec3 fragPosWorld, vec3 Normal, vec3 Diffuse, float Specular)
{
	//discard lights that are not active
	if(!light.isActive) return vec3(0.0);

	//if fragment outside light's radius, return black
	float dist = length(light.Position - FragPos);
	if(dist > light.Radius) return vec3(0.0);

	vec3 lighting = vec3(0.0);
	vec3 viewDir =	normalize(-FragPos);
	vec3 lightDir = normalize(light.Position - FragPos);

	//diffuse
	vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;

	//specular
	vec3 H = normalize(lightDir + viewDir);		//halfway dir vector
	float NdotH = max(dot(Normal, H), 0.0);
	float spec = pow(NdotH, max(1.0, materialShininess)) * Specular;
	vec3 specular = light.Color * spec;

	//shadows
	float shadow = 1.0;
	if(light.CastShadow)
		shadow = PointShadowCalculation(light, lightIndex, FragPos, fragPosWorld);

	//attenuation
	float attenuation = GetAttenuation(dist, light.Radius);
	lighting += (diffuse + specular) * attenuation * shadow * light.Intensity;
	
	return lighting;
}

vec3 Lighting_DirLight(DirectionalLight light, int lightIndex, vec3 FragPos, vec3 fragPosWorld, vec3 Normal, vec3 Diffuse, float Specular)
{
	//discard lights that are not active
	if(!light.isActive) return vec3(0.0);
	
	vec3 viewDir =	normalize(-FragPos);

	//diffuse
	float diff = max(dot(light.Direction, Normal), 0.0);
	vec3 diffuse = diff * light.Color * Diffuse;

	//specular
	vec3 H = normalize(light.Direction + viewDir);		//halfway dir vector
	float NdotH = max(dot(Normal, H), 0.0);
	float spec = pow(NdotH, max(1.0, materialShininess)) * Specular;
	vec3 specular = light.Color * spec;

	//shadows
	float shadow = 1.0;
	if(light.CastShadow)
		shadow = DirShadowCalculation(light, lightIndex, FragPos, fragPosWorld, Normal);
	
	vec3 lighting = vec3(0.0);
	lighting += (diffuse + specular) * shadow * light.Intensity;

	return lighting;
}

void main()
{
	//1 - Early depth reject
	//----------------------
	float Depth = texture(gDepth, TexCoords).r;
	vec3 FragPos = reconstructPosition(Depth);
	if(Depth == 1.0)
		discard;

	//2 - Fetch G-Buffer
	//---------------------
	vec3 Normal = normalize(texture(gNormal, TexCoords).rgb);
	vec3 Diffuse =			texture(gAlbedoSpec, TexCoords).rgb;
	float Specular =		texture(gAlbedoSpec, TexCoords).a;
	float AO =				1.0f;

	if(ssaoEnabled) AO = texture(AOMap, TexCoords).r;

	vec3 fragPosWorld = (inverseViewMatrix * vec4(FragPos, 1.0)).xyz;
	vec3 Ambient = Diffuse * 0.3 * AO;

	//3 - Environment - tinted ambient
	//--------------------------------
	if(irradianceActive)
	{
		//we need world space coordinates to sample the cubemap, so we will convert using the inverse view mat
		vec3 worldNormal = normalize((inverseViewMatrix * vec4(Normal,0.0)).xyz);
		vec3 irradiance = texture(irradianceMap, worldNormal).rgb;

		//HRD tonemapping
		irradiance = irradiance / (irradiance + vec3(1.0));

		//gamma correct
		irradiance = pow(irradiance, vec3(1.0/2.2));

		Ambient *= irradiance;
	}
	//4 - Direct Lights
	//-----------------
	vec3 lighting = vec3(0.0);

	//Ambient
	lighting += Ambient;

	//Point lights
	//make sure loop doesnt exceed array size
	int count = min(numberOfPointLights, MAX_POINT_LIGHTS);	
	for(int i = 0; i < count; ++i)
	{
		if(numberOfPointLights == 0) break;
		lighting += Lighting_PointLight(pointLights[i], i, FragPos, fragPosWorld, Normal, Diffuse, Specular);
	}

	count = min(numberOfDirLights, MAX_DIR_LIGHTS);	
	for(int i = 0; i < count; ++i)
	{
		if(numberOfDirLights == 0) break;
		lighting += Lighting_DirLight(dirLights[i], i, FragPos, fragPosWorld, Normal, Diffuse, Specular);
	}

	FragColor = vec4(lighting, 1.0);
}