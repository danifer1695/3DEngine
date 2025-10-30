#include "IDPass.h"
#include "PostProcessingPass.h"

//=============================================================================================
//Constructors
//=============================================================================================

IDPass::IDPass(unsigned int screenWidth, unsigned int screenHeight)
	:screenWidth{screenWidth}, screenHeight{screenHeight}
{
	idPassShader = std::make_unique<Shader>("IDPASS", ID_SHADER_VS, ID_SHADER_FS);

	SetupFBO();
	SetupTexture();
}
//=============================================================================================
//SetupFBO
//=============================================================================================

void IDPass::SetupFBO()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Utils::getOpenGLError("IDPASS::SETUP_FRAMEBUFFER");
}
//=============================================================================================
//SetupTexture
//=============================================================================================

void IDPass::SetupTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Utils::getOpenGLError("IDPASS::SETUP_TEXTURE");
}

//=============================================================================================
//SetupTexture
//=============================================================================================

void IDPass::Render(Scene& scene, const GBuffer& gbuffer)
{
	idPassShader->use();

	idPassShader->setMatrix4("projection", scene.GetProjectionMatrix());
	idPassShader->setMatrix4("view", scene.GetCamera()->get_view_matrix());
	idPassShader->setInt("gDepth", 0);

	//Bind Textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer.GetGDepthTex());

	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& item : scene.GetItemCollection())
	{
		idPassShader->setMatrix4("model", item.transform.GetModelMatrix());
		idPassShader->setVector3("idColor", item.getIDColor());
		ResourceManager::Get().GetModel(item.getModelHandle())->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Utils::getOpenGLError("IDPASS::RENDER");
}
