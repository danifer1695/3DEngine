#include "PostProcessingPass.h"
//=============================================================================================
//Constructors
//=============================================================================================
PostProcessingPass::PostProcessingPass(unsigned int screenWidth, unsigned int screenHeight)
	:screenWidth{ screenWidth }, screenHeight{ screenHeight }
{
	postProcessingShader = std::make_unique<Shader>("POST_PROCESSING_SHADER", POST_SHADER_VS, POST_SHADER_FS);
	outlineShader = std::make_unique<Shader>("OUTLINE_SHADER", OUTLINE_SHADER_VS, OUTLINE_SHADER_FS);
	iconShader = std::make_unique<Shader>("ICON_SHADER", ICON_SHADER_VS, ICON_SHADER_FS);

	//Handles
	h_lightIconTexture = ResourceManager::Get().GetTextureHandle("Light_Icon");

	SetupFBO();
	SetupTexture();
}

//=============================================================================================
//SetupFBO
//=============================================================================================
void PostProcessingPass::SetupFBO()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Utils::getOpenGLError("POSTPASS::SETUP_FBO");
}
//=============================================================================================
//SetupTexture
//=============================================================================================

void PostProcessingPass::SetupTexture()
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

	Utils::getOpenGLError("POSTPASS::SETUP_TEXTURE");
}
//=============================================================================================
//RenderItemSelection
//=============================================================================================

void PostProcessingPass::RenderItemSelection(Scene& scene)
{
	Item selectedObject = *scene.GetSelectedItem();
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);

	//Write 1 where the stencil and depth test pass (where the object is)
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	glStencilMask(0xFF);
	
	outlineShader->use();
	outlineShader->setMatrix4("model", selectedObject.transform.GetModelMatrix());
	outlineShader->setMatrix4("view", scene.GetCamera()->get_view_matrix());
	outlineShader->setMatrix4("projection", scene.GetProjectionMatrix());
	ResourceManager::Get().GetModel(selectedObject.getModelHandle())->Draw();

	//Render outline object where stencil != 1
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);		//Turn off stencil writing
	glDisable(GL_DEPTH_TEST);	// ensure outline shows

	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.5f, -1.5f);				//Shift polygons forward to avoing z-fighting
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//Render in wireframe mode
	glLineWidth(selectionThickness);			//wireframe lines set to x

	//Draw item again, normally this time
	outlineShader->setVector3("selectColor", selectionColor);
	ResourceManager::Get().GetModel(selectedObject.getModelHandle())->Draw();

	//restore state
	glEnable(GL_DEPTH_TEST);
	glStencilMask(0xFF);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0f);

	glPolygonOffset(0.0f, 0.0f);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_POLYGON_OFFSET_FILL);
}
//=============================================================================================
//SetupTexture
//=============================================================================================

void PostProcessingPass::RenderIcons(Scene& scene)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Precalculate rotation-less view matrix
	glm::mat4 view = scene.GetCamera()->get_view_matrix();
	glm::mat3 noRotView = glm::mat3(view);
	noRotView = glm::transpose(noRotView);

	iconShader->use();
	iconShader->setMatrix4("view", view);
	iconShader->setMatrix4("projection", scene.GetProjectionMatrix());
	iconShader->setInt("iconTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::Get().GetTexture(h_lightIconTexture)->ID);

	for (const auto& light : scene.GetPointLightCollection())
	{
		//Pointer to the light's icon
		Icon* icon = light->GetIcon();

		//Create rotation-less model matrix
		glm::mat4 model = glm::translate(glm::mat4(1.0f), light->transform.getPosition());
		model = glm::scale(model, glm::vec3(icon->GetScale()));

		model *= glm::mat4(noRotView);
		iconShader->setMatrix4("model", model);

		//Draw icon quad
		icon->Draw();
	}

	for (const auto& light : scene.GetDirLightCollection())
	{
		//Pointer to the light's icon
		Icon* icon = light->GetIcon();

		//Create rotation-less model matrix
		glm::mat4 model = glm::translate(glm::mat4(1.0f), light->transform.getPosition());
		model = glm::scale(model, glm::vec3(icon->GetScale()));

		model *= glm::mat4(noRotView);
		iconShader->setMatrix4("model", model);

		//Draw icon quad
		icon->Draw();
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
//=============================================================================================
//SetupTexture
//=============================================================================================

void PostProcessingPass::Render(Scene& scene, GLuint renderTex)
{
	postProcessingShader->use();

	//Setup textures
	postProcessingShader->setInt("renderTex", 0);
	postProcessingShader->setVector2("texelSize", glm::vec2(screenWidth, screenHeight));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTex);


	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	screenQuad.Draw();

	//Setup selected Item's id data
	if (scene.GetSelectedItem() != nullptr) RenderItemSelection(scene);

	//Render Icons
	if(renderIcons) RenderIcons(scene);

	//Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Utils::getOpenGLError("POSTPASS::RENDER");
}