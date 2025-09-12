#include "Renderer.h"

//===============================================================================================
//Constructors
//===============================================================================================

Renderer::Renderer(unsigned int screenWidth, unsigned int screenHeight)
	:screenWidth{screenWidth}, screenHeight{screenHeight}, 
	ssaoPass{screenWidth, screenHeight},
	gBuffer{screenWidth, screenHeight},
	geometryPass{ screenWidth, screenHeight },
	lightingPass{ screenWidth, screenHeight }
{
	Init();
}
//===============================================================================================
//Init()
//===============================================================================================

void Renderer::Init()
{
	SetupFBO();
}
//=============================================================================================
//Render
//=============================================================================================

void Renderer::SetupFBO()
{
	glGenFramebuffers(1, &renderFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);

	glGenTextures(1, &renderTex);
	glBindTexture(GL_TEXTURE_2D, renderTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

	glGenRenderbuffers(1, &renderRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, renderRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	Utils::getOpenGLError("RENDERER::SETUP_FBO");
}
//===============================================================================================
// CopyDepthData();
//===============================================================================================

void Renderer::CopyDepthData()
{
	//Bind source FBO as read source and render FBO as draw destination
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer.GetFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderFBO);

	//Copy depth buffer
	glBlitFramebuffer(
		0, 0, screenWidth, screenHeight,
		0, 0, screenWidth, screenHeight,
		GL_DEPTH_BUFFER_BIT,
		GL_NEAREST
	);

	//Unbind framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//===============================================================================================
// RenderSkybox();
//===============================================================================================

void Renderer::RenderSkybox(const Scene& scene, const GLuint& targetFBO)
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	scene.GetSkybox()->Draw(scene.GetProjectionMatrix(), scene.GetCamera()->get_view_matrix(), targetFBO);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}
//=============================================================================================
//Draw()
//=============================================================================================

void Renderer::Draw(const Scene& scene)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//***IMPORTANT***
	// Keep clear color black, AO calculations rely on the background being this color to discard it
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//We render shadows,Gbuffer and ssao
	shadowPass.Render(scene);
	geometryPass.Render(scene, gBuffer);
	ssaoPass.Render(scene, gBuffer);

	//We copy the GBuffer's depth data over to the renderer's own RBO
	CopyDepthData();

	//We render lighting and skybox to the Renderer's FBO
	lightingPass.Render(
		scene, 
		gBuffer, 
		ssaoPass.GetTexture(), 
		ssaoPass.GetEnabled(), 
		shadowPass.GetDirTextureArray(), 
		shadowPass.GetPointTextureArray(), 
		renderFBO);
	RenderSkybox(scene, renderFBO);

	Utils::getOpenGLError("RENDERER::DRAW");
}