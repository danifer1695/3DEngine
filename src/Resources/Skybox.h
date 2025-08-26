#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"../Shader.h"

#include<stb_image.h>
#include<string>
#include<iostream>

//SHADER DIRECTORIES
#define CUBEMAP_SHADER_VS "./shaders/skybox.vs"
#define CUBEMAP_SHADER_FS "./shaders/skybox.fs"
#define BACKGROUND_SHADER_VS "./shaders/background.vs"
#define BACKGROUND_SHADER_FS "./shaders/background.fs"
#define IRRADIANCE_SHADER_FS "./shaders/renderer/irradiance_convolution.fs"

enum SKYBOX_TYPE 
{
	SKY_IMAGE		= 0,
	SKY_GRADIENT	= 1
};

class Skybox
{
private:
	GLuint captureFBO = 0;
	GLuint captureRBO = 0;
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[6] = {
		//all the angles at which the faces will be captured
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	GLuint skyboxTexture = 0;
	GLuint skyboxCubemap = 0;
	GLuint irradianceCubemap = 0;

	//State
	SKYBOX_TYPE type = SKY_GRADIENT;
	GLuint cubeVAO = 0, cubeVBO = 0;
	unsigned int texWidth, texHeight;

	Shader* hdriToCubemap;
	Shader* backgroundShader;
	Shader* irradianceShader;

	//Initialization
	void Init();
	void SetMatrices(glm::mat4 projection, glm::mat4 view);
	void setupFBO();
	void setupCubemap();
	void setupIrradiance();

	//Helpers
	void renderCube();
	void BindTexture(GLenum textureUnit);
	void RenderCubemap();
	void getError(std::string location);

public:

	//Constructors
	Skybox(SKYBOX_TYPE type,
		unsigned int skyboxTex,
		unsigned int texWidth = 512, 
		unsigned int texHeight = 512);

	//Destructor
	~Skybox();

	//Methods
	void Draw(glm::mat4 projection, glm::mat4 view);
	void BindCubemap(GLenum texUnit);

	//Getters
	const GLuint GetCubemap() const			{ return skyboxCubemap; }
	const GLuint GetIrradianceMap() const	{ return irradianceCubemap; }

	//Debugging
	void SetCubemap(GLint cubemap) { skyboxCubemap = cubemap; }
};

