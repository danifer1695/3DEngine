#include "GeometryPass.h"


//=============================================================================================
//Constructors
//=============================================================================================

GeometryPass::GeometryPass(unsigned int screenWidth, unsigned int screenHeight)
	:screenWidth{screenWidth}, screenHeight{screenHeight}
{
	Initialize();
}
//=============================================================================================
//Initialize()
//=============================================================================================

void GeometryPass::Initialize()
{
	//Create and compile shader
	geomPassShader = std::make_unique<Shader>("RENDERER::GEOMPASS", GEOMPASS_SHADER_VS, GEOMPASS_SHADER_FS);
}
//=============================================================================================
//Render()
//=============================================================================================

void GeometryPass::Render(Scene& scene, const GBuffer& gBuffer)
{
	//Get transform matrices plus camera position
	glm::mat4 projection = scene.GetProjectionMatrix();
	glm::mat4 view = scene.GetCamera()->get_view_matrix();
	glm::vec3 camPos = scene.GetCamera()->Position;

	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.GetFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	geomPassShader->use();
	geomPassShader->setFloat("farPlane", scene.GetFarPlane());
	geomPassShader->setMatrix4("projection", projection);
	geomPassShader->setMatrix4("view", view);

	//Draw all items
	for (auto& obj : scene.GetGameObjectCollection())
	{
		//Dynamic cast to make sure current gameObject is of Item type
		if(auto item = std::dynamic_pointer_cast<Item>(obj))
		{
			//set texture map uniform values to those of the current item's material
			item->sendToShader(*geomPassShader);
			//NEEDS TO CHANGE TO PER-MESH
			item->Draw(projection, view, *geomPassShader.get());
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	Utils::getOpenGLError("GEOMETRYPASS::RENDER");
}