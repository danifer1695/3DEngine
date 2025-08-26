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

}
//=============================================================================================
//Draw()
//=============================================================================================

void Renderer::Draw(const Scene& scene)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shadowPass.Render(scene);
	geometryPass.Render(scene, gBuffer);
	ssaoPass.Render(scene, gBuffer);
	lightingPass.Render(scene, gBuffer, ssaoPass.GetTexture());
	scene.RenderSkybox();

	Utils::getOpenGLError("RENDERER::DRAW");
}