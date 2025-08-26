#include "Quad.h"

//===============================================================================================
//Init()
//===============================================================================================

void Quad::Init()
{
	// setup plane VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	getError("INIT");
}
//===============================================================================================
//Draw()
//===============================================================================================

void Quad::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	
	getError("DRAW");
}
//=============================================================================================
//getError()
//=============================================================================================

void Quad::getError(std::string location)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "RENDERER::" << location << "::OpenGL error: " << err;
		if (err == 1280) std::cerr << " - GL_INVALID_ENUM.";
		else if (err == 1286) std::cerr << " - Invalid Framebuffer Operation.";
		else if (err == 1282) std::cerr << " - GL_INVALID_OPERATION.";
		std::cout << std::endl;
	}
}