#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>
#include<random>

#include"../Core/Utils.h"

enum ShadowType
{
	Point,
	Directional,
	Spotlight
};

class ShadowMap
{
private:
	GLuint fbo;
	GLuint depthMap;

	//Initialization
	void Init(unsigned int width, unsigned int height);

public:
	unsigned int width, height;
	ShadowType type;

	//Constructors
	ShadowMap(ShadowType type, unsigned int width, unsigned int height);

	//Destructor
	~ShadowMap();

	//Methods
	void BindTexture(GLenum textureUnit);
	void BeginCapture();
	void EndCapture();

	//Getters
	unsigned int getDepthMap() const { return depthMap; }
	unsigned int getFBO() const { return fbo; }
};