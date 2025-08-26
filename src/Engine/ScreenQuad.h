#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"../Renderer/Shader.h"

#include<string>

//SHADER DIRECTORIES
#define SCREEN_SHADER_VS "./shaders/screenShader.vs"
#define SCREEN_SHADER_FS "./shaders/screenShader.fs"

class ScreenQuad
{
private:
	GLuint screenWidth, screenHeight;
	GLuint screenFBO, screenRBO;
	GLuint screenTex;
	GLuint quadVAO = 0, quadVBO = 0;

	Shader* screenShader;

	//Helpers
	void setupFBO();
	void setupTexture();
	void getError(std::string location);
	void RenderQuad();

public:
	//Constructors
	ScreenQuad(unsigned int width, unsigned int height);

	//Destructors
	~ScreenQuad();

	//Methods
	void Init();
	void StartCapture();
	void EndCapture();
	void Draw();
	void ToggleEffects(bool toggle) { 
		screenShader->use();
		screenShader->setBool("toggleEffects", toggle); 
		getError("TOGGLE_EFFECTS");
	}

	//Debugging
	void SetTexture(GLuint texture2D) { screenTex = texture2D; }
};

