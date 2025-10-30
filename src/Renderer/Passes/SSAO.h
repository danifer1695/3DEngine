#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<vector>
#include<random>

#include"../GBuffer.h"
#include"../../Engine/Scene.h"
#include"../../Resources/Primitives/Quad.h"
#include"../../Core/Utils.h"

#define SSAO_VS			"./shaders/renderer/ssao.vs"
#define SSAO_FS			"./shaders/renderer/ssao.fs"
#define SSAO_BLUR_FS	"./shaders/renderer/ssao_blur.fs"

class SSAOPass
{
private:
	bool ssaoEnabled = true;
	unsigned int screenWidth, screenHeight;

	GLuint ssaoFBO, ssaoBlurFBO;
	GLuint ssaoColorBuffer, ssaoBlurColorBuffer;
	GLuint noiseTex;

	std::vector<glm::vec3> kernel;
	unsigned int sampleNr = 16;

	std::unique_ptr<Shader> ssaoShader;
	std::unique_ptr<Shader> ssaoBlurShader;

	Quad screenQuad;

	//Initialization
	void Init();
	void SetupShaders();
	void SetupFBO();
	void GenerateKernel();
	void GenerateNoiseTexture();

	//Rendering
	void GenerateSSAOTex(const Scene& scene, const GBuffer& gBuffer);
	void GenerateBlurSSAO();

public:
	//Constructors
	SSAOPass(unsigned int screenWidth, unsigned int screenHeight);

	//Render
	void Render(const Scene& scene, const GBuffer& gBuffer);

	//Getters
	const GLuint	GetTexture() const { return ssaoBlurColorBuffer; }
	const bool		GetEnabled() const { return ssaoEnabled; }

	//Setters
	void SetEnabled(bool set) { ssaoEnabled = set; }
};

