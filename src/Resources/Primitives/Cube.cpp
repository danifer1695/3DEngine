#include "Cube.h"

//===============================================================================================
//Initialize()
//===============================================================================================

void Cube::Init()
{
	ExtractVertexInfo();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}
//===============================================================================================
//Draw()
//===============================================================================================

void Cube::ExtractVertexInfo()
{
	//Extract position and texture coordinate vectors from cubeVertices
	unsigned int vertCount = arrayLength / 5;
	for (int i = 0; i < vertCount; i++)
	{
		glm::vec3 pos = glm::vec3(cubeVertices[i * 5 + 0], cubeVertices[i * 5 + 1], cubeVertices[i * 5 + 2]);
		glm::vec2 tex = glm::vec2(cubeVertices[i * 5 + 3], cubeVertices[i * 5 + 4]);
		
		positions.push_back(pos);
		texCoords.push_back(tex);
	}
}
//===============================================================================================
//Draw()
//===============================================================================================

void Cube::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}