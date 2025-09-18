#pragma once
#include"../../Engine/Scene.h"
#include"../../Core/Utils.h"

#include <memory>

//SHADER DIRECTORIES
#define ID_SHADER_VS "./shaders/renderer/idPass.vs"
#define ID_SHADER_FS "./shaders/renderer/idPass.fs"

class IDPass
{
private:
	//Data
	unsigned int screenWidth, screenHeight;

	GLuint FBO;
	GLuint RBO;
	GLuint texture;

	std::unique_ptr<Shader> idPassShader;

	//Initialization
	void SetupFBO();
	void SetupTexture();

public:
	//Constructor
	IDPass(unsigned int screenWidth, unsigned int screenHeight);

	//Render
	void Render(Scene& scene);

	//Getters
	const GLuint GetTexture() const { return texture; }
};

