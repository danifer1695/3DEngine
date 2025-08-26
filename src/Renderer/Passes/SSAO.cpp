#include "SSAO.h"

//=============================================================================================
//Constructors()
//=============================================================================================

SSAOPass::SSAOPass(unsigned int screenWidth, unsigned int screenHeight)
	:screenWidth{screenWidth}, screenHeight{screenHeight}
{
	Init();
}
//=============================================================================================
//Init()
//=============================================================================================

void SSAOPass::Init()
{
	SetupShaders();
	SetupFBO();
	GenerateKernel();
	GenerateNoiseTexture();
}
//=============================================================================================
//SetupShaders()
//=============================================================================================

void SSAOPass::SetupShaders()
{
	ssaoShader = std::make_unique<Shader>("SSAOPass", SSAO_VS, SSAO_FS);
	ssaoBlurShader = std::make_unique<Shader>("SSAO_BLUR", SSAO_VS, SSAO_BLUR_FS);

	ssaoShader->use();
	ssaoShader->setInt("gPosition", 0);
	ssaoShader->setInt("gNormal", 1);
	ssaoShader->setInt("texNoise", 2);
	ssaoShader->setInt("sampleNr", sampleNr);
	ssaoShader->setFloat("screenWidth", (float)screenWidth);
	ssaoShader->setFloat("screenHeight", (float)screenHeight);

	ssaoBlurShader->use();
	ssaoBlurShader->setInt("ssaoInput", 0);

	getError("SETUP_SHADERS");
}
//=============================================================================================
//SetupFBO()
//=============================================================================================

void SSAOPass::SetupFBO()
{
	//SSAO color buffer
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	//AO only uses R channel
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, screenWidth, screenHeight, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAOPass Framebuffer not complete!" << std::endl;

	getError("SETUP_FBO::SSAOPass");

	//Blur color buffer
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);

	glGenTextures(1, &ssaoBlurColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoBlurColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, screenWidth, screenHeight, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBlurColorBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAOPass Blur Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	getError("SETUP_FBO::BLUR");
}
//=============================================================================================
//GenerateKernel()
//=============================================================================================

void SSAOPass::GenerateKernel()
{
	//lerping lambda function
	auto lerp = [](float a, float b, float f) {
		return a + f * (b - a);
		};

	//kernel is in tangent space, so Z points up, not Y
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);	//generates random number between [0.0, 1.0]
	std::default_random_engine generator;

	for (size_t i = 0; i < sampleNr; ++i)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,	//multiply by 2.0 and deduct 1.0 to convert to range [-1.0, 1.0]
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)		//hemisphere only needs z values in range [0.0, 1.0]
		);

		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0f;

		//we make small values smaller but keep large values relatively large
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		kernel.push_back(sample);
	}

	getError("GENERATE_KERNEL");
}
//=============================================================================================
//GenerateNoiseTexture()
//=============================================================================================

void SSAOPass::GenerateNoiseTexture()
{
	std::vector<glm::vec3>ssaoNoise;
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);	//generates random number between [0.0, 1.0]
	std::default_random_engine generator;

	//The texture will be 4x4, tiled across the entire screen.
	//It will serve to five each Kernel a different rotation angle
	for (unsigned int i = 0; i < 16; ++i)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,	//x value (tangent space)
			randomFloats(generator) * 2.0 - 1.0,	//y value
			0.0f);									//z value. we rotate around this axis, so we leave it at 0
		ssaoNoise.push_back(noise);
	}

	glGenTextures(1, &noiseTex);
	glBindTexture(GL_TEXTURE_2D, noiseTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	getError("GENERATE_NOISE_TEXTURE");
}
//=============================================================================================
//GenerateSSAOTex()
//=============================================================================================

void SSAOPass::GenerateSSAOTex(const Scene& scene, const GBuffer& gBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	ssaoShader->use();
	for (unsigned int i = 0; i < sampleNr; ++i)
		ssaoShader->setVector3("samples[" + std::to_string(i) + "]", kernel[i]);
	ssaoShader->setMatrix4("projection", scene.GetProjectionMatrix());

	//Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetGPositionTex());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetGNormalTex());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTex);

	screenQuad.Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	getError("GENERATE_SSAO_TEXTURE");
}
//=============================================================================================
//GenerateBlurSSAO()
//=============================================================================================
void SSAOPass::GenerateBlurSSAO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	ssaoBlurShader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);

	screenQuad.Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//=============================================================================================
//Render()
//=============================================================================================

void SSAOPass::Render(const Scene& scene, const GBuffer& gBuffer)
{
	GenerateSSAOTex(scene, gBuffer);
	GenerateBlurSSAO();
}
//=============================================================================================
//getError()
//=============================================================================================

void SSAOPass::getError(std::string location)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "SSAOPass::" << location << "::OpenGL error: " << err;
		if (err == 1280) std::cerr << " - GL_INVALID_ENUM.";
		else if (err == 1286) std::cerr << " - Invalid Framebuffer Operation.";
		else if (err == 1282) std::cerr << " - GL_INVALID_OPERATION.";
		std::cout << std::endl;
	}
}