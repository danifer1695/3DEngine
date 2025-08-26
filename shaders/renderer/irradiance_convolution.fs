#version 330 core

out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{
	//the hemisphere's orientation will match the fragment's sample direction.
	//this means N is the vector pointing up towards the north pole of the hemisphere in
	//	world space, or in other words, its normal vector. WorldPos being the world space 
	//	position of the current fragment
	vec3 N = normalize(WorldPos);

	vec3 irradiance = vec3(0.0);

	//tangent space calculation from origin point
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, N));
	up = normalize(cross(N, right));

	//the delta is the distance between samples. the higher the delta, the more samples we
	//	take, the more accurate the convolution will be.
	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	
	//IMPORTANT TO NOTE
	//In this tangent space we built, **Z POINTS UP** towards the normal.
	//-------------------------------------------------------------------

	//We loop to get samples at different spots on the sampling hemisphere
	//Phi represents the angle around the Z axis (called the azimuthal angle, it 
	//	rotates the vector horizontally along one of the sphere's altitude rings
	//	on the XY plane)
	//We use 'phi' to sample around a ring on the hemisphere's surface between 0
	//	and 2*pi.
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		//Theta represents the angle from the Z axis (called the polar angle, it rotates the 
		//vector vertically north-south along the sphere)
		//We use theta to jump between rings on the hemisphere's surface. Think of
		//	rings as altitude lines on Earth. we sample from 0 (north pole) to 0.5*pi
		//	(the equator). Notice how that distance makes exactly 1/4
		//	of a circle's circumference - hence why we loop up to 0.5*pi 
		//	(1/4 of 2*pi is 0.5*pi)
		for(float theta = 0.0; theta < 0.5 * PI; theta +=sampleDelta)
		{
			//spherical to cartesian (in tangent space)
			//This means, from angles along the sphere to xyz coordinates (again, in tangent
			//	space. Basically we are taking an angle pair (theta, pi) and transforming it
			//	into a directional vector with its origin at the center of the 
			//	sphere (tangentSample in tangent space, sampleVec in world space)
			vec3 tangentSample = vec3(sin(theta) * cos(phi), 
									  sin(theta) * sin (phi), 
									  cos(theta));
			//tangent to world space. we will use it to sample the HDRi map
			vec3 sampleVec = tangentSample.x * right + 
							 tangentSample.y * up + 
							 tangentSample.z * N;

			//Notice the (*cos(theta) * sin(theta)). Here is why that's in the equation:
			//Imagine a sphere divided into a grid, like how earth is divided by longitude and
			//	latitude lines. that sphere is now divided into many little squares. But you
			//	will notice that as we move north or south on the sphere, those squares get
			//	smaller and smaller the closer they get to the poles. 
			//If we are to take a color sample  for each of those squares in our hemisphere,
			//	because of how densely concentrated the squares near the pole are compared to
			//	those near the equator, we cannot just average all those color samples equally.
			//	The colors along the equator would have a lot less representation in the final
			//	averaged color than those near the north. So to remedy that  we scale the sampled
			//	color value by its spherical coordinates, scaling down values near the pole and
			//	amplifying those around the equator.
			irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	//Now that we have added all samples to irradiance, we will average the value by
	//	dividing the total by the number of samples taken.
	//The result is the average color among all the sampled points on the HDRi map.
	irradiance = PI * irradiance * (1.0 / float(nrSamples));

	FragColor = vec4(irradiance, 1.0);
	//FragColor = vec4(1.0, 0.3, 1.0, 1.0);
}