#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<vector>
#include<random>

#include"../../Shader.h"
#include"../../Engine/Scene.h"
#include"../../Primitives/Quad.h"
#include"../GBuffer.h"

#define SSAO_VS			"./shaders/renderer/ssao.vs"
#define SSAO_FS			"./shaders/renderer/ssao.fs"
#define SSAO_BLUR_FS	"./shaders/renderer/ssao_blur.fs"

class SSAOPass
{
private:
	unsigned int screenWidth, screenHeight;

	GLuint ssaoFBO, ssaoBlurFBO;
	GLuint ssaoColorBuffer, ssaoBlurColorBuffer;
	GLuint noiseTex;

	std::vector<glm::vec3> kernel;
	unsigned int sampleNr = 64;

	std::unique_ptr<Shader> ssaoShader;
	std::unique_ptr<Shader> ssaoBlurShader;

	Quad screenQuad;

	//private methods
	void getError(std::string location);

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
	const GLuint GetTexture() const { return ssaoBlurColorBuffer; }
};

