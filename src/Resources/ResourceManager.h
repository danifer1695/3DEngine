#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

#include"../../../stb_image.h"

class ResourceManager
{
private:
	//Constructor
	ResourceManager() {};

public:

	//We delete copy constructor and assignment operator
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	//Get instance
	static ResourceManager& Get();

	unsigned int loadTexture(char const* path, bool gammaCorrection);
	unsigned int loadHDRi(char const* path);
};

