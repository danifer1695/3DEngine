#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;	//per EmitVertex()

//this geometry shader takes in each of the model's triangles and transfroms
//	them into light space one by one
void main()
{
	//we draw each triangle through each of the 6 cubemap faces
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = face;	//gl_Layer specifies which cubemap face we render to
		for(int i = 0; i < 3; ++i)
		{
			//this returns the position of each of the triangle vertices, 'i' range = [0, 2]
			FragPos = gl_in[i].gl_Position;	
			//transform coordinates to light space using each one of the view matrices we created
			//for each of the cubemap's faces
			gl_Position = shadowMatrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}