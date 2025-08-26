#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

//We take in the information output by the gbuffer (geometry pass)
uniform sampler2D gPosition;					//check: set
uniform sampler2D gNormal;						//check: set
uniform sampler2D gAlbedoSpec;					//check: set
uniform sampler2D AOMap;						//check: set

//Environment
uniform bool irradianceActive;					//check: set
uniform samplerCube irradianceMap;				//check: set

//Camera & Material
uniform float materialShininess;				//check: set
uniform float farPlane;							//check: set
uniform mat4 inverseViewMatrix;					//check: set
	
//Light info
struct PointLight
{
	vec3 Position;								//check: set
	vec3 Color;									//check: set
	float Intensity;							//check: set
	float Radius;								//check: set

	bool CastShadow;							//check: set
	samplerCube ShadowMap;						//check: set
};

const int MAX_LIGHTS = 32;
uniform int numberOfLights;						//check: set
uniform PointLight pointLights[MAX_LIGHTS];		//check: set

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

float PointShadowCalculation(PointLight light, vec3 FragPos)
{
	vec3 lightPosWorld = (inverseViewMatrix * vec4(light.Position, 1.0)).xyz;
	vec3 fragPosWorld = (inverseViewMatrix * vec4(FragPos, 1.0)).xyz;

	vec3 lightToFrag = fragPosWorld - lightPosWorld;	//both should be in view space
	float depth = length(lightToFrag);
	lightToFrag = normalize(lightToFrag);			//we will use this to sample the cubemap
	//vec3 samplingVector = (inverse(viewMatrix) * vec4(lightToFrag, 1.0)).xyz;

	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float lightDistance = depth;
	float diskRadius = (1.0 + (lightDistance / farPlane)) / 105.0;

	//Random rotation matrix
	vec3 randVec = randomUnitVector(fragPosWorld);
	mat3 rot = buildRotationMatrix(randVec);

	//make sure index doesnt surpass array size
	int count = min(samples, MAX_SAMPLES);		
	for(int i = 0; i < count; ++i)
	{
		vec3 offset = rot * sampleOffsets[i];
		float sampledDepth = texture(light.ShadowMap, lightToFrag + offset * diskRadius).r;
		sampledDepth *= farPlane;

		if(depth - bias > sampledDepth)
			shadow += 1.0;
	}

	shadow /= float(count);
	return 1.0 - shadow;
	//return texture(light.ShadowMap, lightToFrag).r;
	//return depth / 30.0;
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

vec3 Lighting_PointLight(PointLight light, vec3 FragPos, vec3 Normal, vec3 Diffuse, float Specular)
{
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
		shadow = PointShadowCalculation(light, FragPos);

	//attenuation
	float attenuation = GetAttenuation(dist, light.Radius);
	lighting += (diffuse + specular) * attenuation * shadow * light.Intensity;
	

	return lighting;
	//return vec3(shadow);		//debugging
}

void main()
{
	//1 - Early depth reject
	//----------------------
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	float Depth = FragPos.z;
	//discard fragments that are at depth 1.0 or beyond
	if(Depth == 0)
		discard;

	//2 - Fetch G-Buffer
	//---------------------
	vec3 Normal = normalize(texture(gNormal, TexCoords).rgb);
	vec3 Diffuse =			texture(gAlbedoSpec, TexCoords).rgb;
	float Specular =		texture(gAlbedoSpec, TexCoords).a;
	float AO =				texture(AOMap, TexCoords).r;

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

	int count = min(numberOfLights, MAX_LIGHTS);	//make sure loop doesnt exceed array size
	for(int i = 0; i < count; ++i)
	{
		lighting += Lighting_PointLight(pointLights[i], FragPos, Normal, Diffuse, Specular);
	}

	//lighting = Lighting_PointLight(pointLights[0], FragPos, Normal, Diffuse, Specular);	//debugging

	FragColor = vec4(lighting, 1.0);
	//FragColor = vec4(FragPos + 1.0, 1.0);	//debugging
}