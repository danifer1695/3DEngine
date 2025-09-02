#include "LightingPass.h"

//=============================================================================================
//Constructors
//=============================================================================================

LightingPass::LightingPass(unsigned int screenWidth, unsigned int screenHeight)
	:screenWidth{ screenWidth }, screenHeight{ screenHeight }
{
	Initialize();
}
//=============================================================================================
//Initialize
//=============================================================================================

void LightingPass::Initialize()
{
	//Build and compile shaders
	//-------------------------
	lightPassShader = std::make_unique<Shader>("RENDERER::LIGHTPASS", LIGHTPASS_SHADER_VS, LIGHTPASS_SHADER_FS);
	
	Utils::getOpenGLError("LIGHTINGPASS::SHADER_CONFIG::SHADER_COMPILATION");

	//Configure uniform values
	//------------------------
	lightPassShader->use();
	lightPassShader->setBool("irradianceActive", irradianceActive);

	//Texture Unit Assignment
	//Texture units 0-2 reserved for G-buffer
	lightPassShader->setInt("gPosition", 0);
	lightPassShader->setInt("gNormal", 1);
	lightPassShader->setInt("gAlbedoSpec", 2);
	//Texture units 3 for AO
	lightPassShader->setInt("AOMap", 3);
	//Texture units 4 for IBL
	lightPassShader->setInt("irradianceMap", 4);
	//Texture units 5-beyond for shadow maps

	Utils::getOpenGLError("LIGHTINGPASS::SHADER_CONFIG::UNIFORM_SETTINGS");
}
//=============================================================================================
//Render
//=============================================================================================

void LightingPass::Render(const Scene& scene, const GBuffer& gBuffer, const GLuint& ssaoTex, const GLuint& targetFBO)
{
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::COMING_FROM_ABOVE");
	//vector for each type of light (this will to be moved to the light class)
	std::vector<Light*>plVec;
	std::vector<Light*>dlVec;

	for (auto& light : scene.GetLightCollection())
	{
		if (light->GetLightType() == POINT_LIGHT)
		{
			plVec.push_back(light.get()); 
			continue;
		}
		else if (light->GetLightType() == DIRECTIONAL_LIGHT)
		{
			dlVec.push_back(light.get()); 
			continue;
		}
	}

	lightPassShader->use();
	lightPassShader->setInt("numberOfPointLights",	(int)plVec.size());
	lightPassShader->setInt("numberOfDirLights",	(int)dlVec.size());
	lightPassShader->setInt("numberOfSpotLights",	0);			//***NEEDS UPDATING WHEN IMPLEMENTING SPOTLIGHTS***
	lightPassShader->setFloat("farPlane", scene.GetFarPlane());
	lightPassShader->setFloat("materialShininess", 16.0f);
	lightPassShader->setMatrix4("inverseViewMatrix", glm::inverse(scene.GetCamera()->get_view_matrix()));
	lightPassShader->setMatrix4("viewMatrix", scene.GetCamera()->get_view_matrix());
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::UNIFORM_SETUP");

	//bind all gbuffer textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetGPositionTex());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetGNormalTex());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetGAlbedoSpecTex());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ssaoTex);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, scene.GetSkybox()->GetIrradianceMap());
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::TEX_BINDING");

	//start index at GL_TEXTURE5
	int texUnitIndex = 5;

	//point lights info to shader
	for (size_t i = 0; i < plVec.size(); ++i)
	{
		//conditional: if GL_TEXTURE0 + texUnitIndex exceeds GL_TEXTURE31, break the loop
		if (texUnitIndex > 31) break;
		auto* pl = dynamic_cast<PointLight*>(plVec.at(i));

		SendPointLightToShader(pl, scene, texUnitIndex, i);

		texUnitIndex++;
	}

	//send directional lights info to shader
	for (size_t i = 0; i < dlVec.size(); ++i)
	{
		//conditional: if GL_TEXTURE0 + texUnitIndex exceeds GL_TEXTURE31, break the loop
		if (texUnitIndex > 31) break;
		auto* dl = dynamic_cast<DirectionalLight*>(dlVec.at(i));

		SendDirLightToShader(dl, scene, texUnitIndex, i);

		texUnitIndex++;
	}
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::LIGHTINFO_TO_SHADER");

	//bind Texture framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, targetFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::FBO_SETUP");

	screenQuad.Draw();

	Utils::getOpenGLError("LIGHTINGPASS::RENDER");
}
//=============================================================================================
//SendPointLightToShader
//=============================================================================================

void LightingPass::SendPointLightToShader(PointLight* pl, const Scene& scene, int texUnitIndex, int lightIndex)
{
	GLint texUnit = GL_TEXTURE0 + texUnitIndex;

	pl->sendToShader(
		*lightPassShader,								//shader
		"pointLights[" + std::to_string(lightIndex) + "]",		//uniform name
		scene.GetCamera()->get_view_matrix(),			//view matrix
		texUnitIndex);									//texture unit

	//Bind ShadowMap Texture
	pl->GetShadowMap()->BindTexture(texUnit);
}
//=============================================================================================
//SendDirLightToShader
//=============================================================================================

void LightingPass::SendDirLightToShader(DirectionalLight* dl, const Scene& scene, int texUnitIndex, int lightIndex)
{
	GLint texUnit = GL_TEXTURE0 + texUnitIndex;

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, scene.GetNearPlane(), scene.GetFarPlane());
	glm::mat4 lightView = glm::lookAt(dl->transform.getPosition(), dl->GetTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	dl->sendToShader(
		*lightPassShader,								//shader
		"dirLights[" + std::to_string(lightIndex) + "]",			//uniform name
		scene.GetCamera()->get_view_matrix(),			//view matrix
		texUnitIndex,									//texture unit
		lightSpaceMatrix);								//light space matrix

	//Bind ShadowMap Texture
	dl->GetShadowMap()->BindTexture(texUnit);
}
