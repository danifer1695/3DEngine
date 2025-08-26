#version 330 core
//the camera will be pointing at one of the faces of a cube, from the inside.
// it will project and capture part of the hdri onto that face
out vec4 FragColor;

in vec3 WorldPos;

uniform sampler2D hdri;

//(0.1591, 0.3183) = (1/2*pi, 1/pi)
const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
	//3D world position to spherical 2D uvs
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main()
{
	vec2 uv = SampleSphericalMap(normalize(WorldPos));
	vec3 color = texture(hdri, uv).rgb;

	FragColor = vec4(color, 1.0);
}