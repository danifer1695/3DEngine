#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform samplerCube depthCubemap;   // your point light shadow map
uniform vec3 lightPos;              // position of the light (the cube map center)
uniform vec3 testDir;               // direction you want to sample from
uniform float farPlane;             // the far plane you used when rendering shadow map
uniform float nearPlane;             // the far plane you used when rendering shadow map


// Convert texcoords into a direction vector for the -Y face
vec3 texCoordToDir(vec2 uv)
{
    // remap [0,1] -> [-1,1]
    vec2 st = uv * 2.0 - 1.0;
    return normalize(vec3(st.x, -1.0, st.y)); 
    // -Y face: x maps to +u, y = -1 (fixed), z maps to +v
}

//required when using a perspective projection matrix
float LinearizeDepth(float depth){
	float z = depth * 2.0 - 1.0; //back to NDC
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
    vec3 dir = texCoordToDir(TexCoords);

    float depth = texture(depthCubemap, dir).r;
    //depth *= farPlane;

    //float visual = depth / farPlane;
    FragColor = vec4(vec3(depth), 1.0);
}