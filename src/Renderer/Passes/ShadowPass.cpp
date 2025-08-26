#include "ShadowPass.h"

//=============================================================================================
//Constructors
//=============================================================================================

ShadowPass::ShadowPass()
{
	Initialize();
}
//=============================================================================================
//Initialize
//=============================================================================================

void ShadowPass::Initialize()
{
	//Build and compile shaders
	pointShadowShader = std::make_unique<Shader>("RENDERER::POINT_SHADOW", POINT_SHADOW_SHADER_VS, POINT_SHADOW_SHADER_FS, POINT_SHADOW_SHADER_GS);
}
//=============================================================================================
//Render
//=============================================================================================

void ShadowPass::Render(const Scene& scene)
{
	float nearPlane = scene.GetNearPlane();
	float farPlane = scene.GetFarPlane();
	size_t numberOfLights = scene.GetLightCollection().size();
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);

	//We loop through all of the scene's lights
	for (size_t i = 0; i < numberOfLights; ++i)
	{
		//skip if light casts no shadows
		if (!scene.GetLightCollection().at(i)->castShadows) continue;

		//if static_cast to PointLight on current Light does not return nullptr, its a point light
		if (auto* pl = dynamic_cast<PointLight*>(scene.GetLightCollection()[i].get()))
		{
			glm::vec3 lightPos = pl->transform.getPosition();

			//Create transform matrices for shadow capturing
			std::vector<glm::mat4> shadowTransforms;	//projection * view to transform to view space
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

			pl->GetShadowMap()->BeginCapture();
			pointShadowShader->use();
			pointShadowShader->setFloat("farPlane", farPlane);
			pointShadowShader->setVector3("lightPos", lightPos);
			//send transform matrices to shader
			for (size_t i = 0; i < 6; ++i)
			{
				pointShadowShader->setMatrix4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
			}

			//Draw all objects
			for (const auto& item : scene.GetItemCollection()) {
				pointShadowShader->setMatrix4("model", item.second->transform.GetModelMatrix());
				item.second->getModel()->Draw();
			}
			pl->GetShadowMap()->EndCapture();
		}

		//Other shadow types will be processed here once implemented
		//[...]
	}
}