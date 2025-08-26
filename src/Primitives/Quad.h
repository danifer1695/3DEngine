#pragma once

#include<string>
#include<iostream>

#include "../Core/GameObject.h"

class Quad : public GameObject
{
private:
	GLuint VAO = 0, VBO = 0;

	float quadVertices[20] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

public:
	//Constructors
	Quad() { Init(); }

	//Initialization
	void Init();

	//Render
	void Draw();

	//debugging
	void getError(std::string location);
};

