#pragma once

#include<memory>

#include"../Core/Utils.h"

class GBuffer
{
private:
	//State
	unsigned int screenWidth;
	unsigned int screenHeight;

	//Buffers
	GLuint FBO;
	GLuint RBO;

	//Textures
	GLuint gPosition;
	GLuint gNormal;
	GLuint gAlbedoSpec;

	//Initialization
	void Init();

public:
	//Constructors
	GBuffer(unsigned int screenWidth, unsigned int screenHeight);

	//Destructor
	~GBuffer();

	//Getters
	const GLuint GetFBO() const				{ return FBO; }
	const GLuint GetGPositionTex() const	{ return gPosition; }
	const GLuint GetGNormalTex() const		{ return gNormal; }
	const GLuint GetGAlbedoSpecTex() const	{ return gAlbedoSpec; }
};

