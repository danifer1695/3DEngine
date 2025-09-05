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
	dirShadowShader = std::make_unique<Shader>("RENDERER::DIRECTIONAL_SHADOW", DIR_SHADOW_SHADER_VS, DIR_SHADOW_SHADER_FS);
}
//=============================================================================================
//Render
//=============================================================================================

void ShadowPass::Render(const Scene& scene)
{
	bool needsUpdate = false;
	bool globalUpdate = false;

	//Check Items
	for (const auto& item : scene.GetItemCollection())
	{
		if (item.second->transform.GetIsDirty())
		{
			needsUpdate = true;
			globalUpdate = true;
			break;
		}
	}

	//Check lights if items are all clean
	if (!needsUpdate)
	{
		for (const auto& light : scene.GetLightCollection())
		{
			if (light->transform.GetIsDirty())
			{
				needsUpdate = true;
				break;
			}
		}
	}

	//If scene is not clean, update shadows, and reset dirty flags
	if (needsUpdate)
	{
		UpdateShadows(scene, globalUpdate);
		ResetDirtyFlags(scene);
	}
}
//=============================================================================================
//ResetDirtyFlags()
//=============================================================================================

void ShadowPass::ResetDirtyFlags(const Scene& scene)
{
	//Reset dirty flags
	for (const auto& item : scene.GetItemCollection())
	{
		item.second->transform.SetIsDirty(false);
	}
	for (const auto& light : scene.GetLightCollection())
	{
		light->transform.SetIsDirty(false);
	}
}
//=============================================================================================
//UpdateShadows
//=============================================================================================

void ShadowPass::UpdateShadows(const Scene& scene, bool globalUpdate)
{
	float nearPlane = scene.GetNearPlane();
	float farPlane = scene.GetFarPlane();
	size_t numberOfLights = scene.GetLightCollection().size();
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);

	//We loop through all of the scene's lights
	for (size_t i = 0; i < numberOfLights; ++i)
	{
		//skip if light casts no shadows or if its not dirty
		if (!scene.GetLightCollection().at(i)->castShadows) continue;
		//if update isn't global and shadow isnt dirty, we skip
		if (!globalUpdate && !scene.GetLightCollection().at(i)->transform.GetIsDirty()) continue;

		//if static_cast to PointLight on current Light does not return nullptr, its a point light
		if (scene.GetLightCollection().at(i)->GetLightType() == POINT_LIGHT)
		{
			auto* pl = dynamic_cast<PointLight*>(scene.GetLightCollection()[i].get());
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
			std::cout << "Rendering Point Shadows" << std::endl;
			for (const auto& item : scene.GetItemCollection()) {
				pointShadowShader->setMatrix4("model", item.second->transform.GetModelMatrix());
				item.second->getModel()->Draw();
			}
			pl->GetShadowMap()->EndCapture();
		}

		else if (scene.GetLightCollection().at(i)->GetLightType() == DIRECTIONAL_LIGHT)
		{
			auto* dl = dynamic_cast<DirectionalLight*>(scene.GetLightCollection()[i].get());
			glm::vec3 lightPos = dl->transform.getPosition();
			glm::vec3 lightTarget = dl->GetTarget();

			glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, scene.GetNearPlane(), scene.GetFarPlane());
			glm::mat4 lightView = glm::lookAt(lightPos, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 lightSpaceMatrix = lightProjection * lightView;

			dirShadowShader->use();
			dirShadowShader->setMatrix4("lightSpaceMatrix", lightSpaceMatrix);

			dl->GetShadowMap()->BeginCapture();

			//Draw all items in the scene
			std::cout << "Rendering Direct Shadows" << std::endl;
			for (const auto& item : scene.GetItemCollection()) {
				dirShadowShader->setMatrix4("model", item.second->transform.GetModelMatrix());
				item.second->getModel()->Draw();
			}

			dl->GetShadowMap()->EndCapture();
		}
	}
}
