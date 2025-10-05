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
	lightPassShader = std::make_unique<Shader>("LIGHTPASS", LIGHTPASS_SHADER_VS, LIGHTPASS_SHADER_FS);
	
	Utils::getOpenGLError("LIGHTINGPASS::SHADER_CONFIG::SHADER_COMPILATION");

	//Configure uniform values
	//------------------------
	lightPassShader->use();
	lightPassShader->setBool("irradianceActive", irradianceActive);

	//Texture Unit Assignment
	//Texture units 0-2 reserved for G-buffer
	lightPassShader->setInt("gDepth",			0);
	lightPassShader->setInt("gNormal",			1);		//Glossiness included in 'a' channel
	lightPassShader->setInt("gAlbedoSpec",		2);
	lightPassShader->setInt("gEmissive",		3);
	//Texture units 3 for AO
	lightPassShader->setInt("AOMap",			4);
	//Texture units 4 for IBL
	lightPassShader->setInt("irradianceMap",	5);
	//Texture units 5-beyond for shadow maps
	lightPassShader->setInt("dirShadowArray",	6);
	lightPassShader->setInt("pointShadowArray", 7);

	Utils::getOpenGLError("LIGHTINGPASS::SHADER_CONFIG::UNIFORM_SETTINGS");
}
//=============================================================================================
//Render
//=============================================================================================

void LightingPass::Render(Scene& scene, const GBuffer& gBuffer, const GLuint& ssaoTex, const bool& ssaoEnabled, const GLuint& dirShadowArray, const GLuint& pointShadowArray, const GLuint& targetFBO)
{
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::COMING_FROM_ABOVE");

	lightPassShader->use();
	lightPassShader->setBool("ssaoEnabled",				ssaoEnabled);
	lightPassShader->setInt("numberOfPointLights",		scene.GetPointLightCollection().size());
	lightPassShader->setInt("numberOfDirLights",		scene.GetDirLightCollection().size());
	lightPassShader->setInt("numberOfSpotLights",		0);			//***NEEDS UPDATING WHEN IMPLEMENTING SPOTLIGHTS***
	lightPassShader->setFloat("farPlane",				scene.GetFarPlane());
	lightPassShader->setFloat("emissiveIntensity",		1.0f);
	lightPassShader->setMatrix4("inverseViewMatrix",	glm::inverse(scene.GetCamera()->get_view_matrix()));
	lightPassShader->setMatrix4("inverseProjMatrix",	glm::inverse(scene.GetProjectionMatrix()));
	lightPassShader->setMatrix4("viewMatrix",			scene.GetCamera()->get_view_matrix());
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::UNIFORM_SETUP");

	//bind all gbuffer textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetGDepthTex());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetGNormalTex());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetGAlbedoSpecTex());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gBuffer.GetGEmissiveTex());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ssaoTex);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, scene.GetSkybox()->GetIrradianceMap());
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D_ARRAY, dirShadowArray);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, pointShadowArray);
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::TEX_BINDING");

	//point lights info to shader
	for (size_t i = 0; i < scene.GetPointLightCollection().size(); ++i)
	{
		//conditional: if GL_TEXTURE0 + texUnitIndex exceeds GL_TEXTURE31, break the loop
		if (scene.GetPointLightCollection().size() == 0) break;
		auto* pl = dynamic_cast<PointLight*>(scene.GetPointLightCollection().at(i).get());

		SendPointLightToShader(pl, scene, i);
	}

	//send directional lights info to shader
	for (size_t i = 0; i < scene.GetDirLightCollection().size(); ++i)
	{
		if (scene.GetDirLightCollection().size() == 0) break;
		auto* dl = dynamic_cast<DirectionalLight*>(scene.GetDirLightCollection().at(i).get());

		SendDirLightToShader(dl, scene, i);
	}
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::LIGHTINFO_TO_SHADER");

	//bind render framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, targetFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	Utils::getOpenGLError("LIGHTINGPASS::RENDER::FBO_SETUP");

	screenQuad.Draw();

	Utils::getOpenGLError("LIGHTINGPASS::RENDER");
}
//=============================================================================================
//SendPointLightToShader
//=============================================================================================

void LightingPass::SendPointLightToShader(PointLight* pl, Scene& scene, size_t lightIndex)
{
	lightPassShader->use();
	pl->sendToShader(
		*lightPassShader,									//shader
		lightIndex,	//uniform name
		scene.GetCamera()->get_view_matrix());				//view matrix
}
//=============================================================================================
//SendDirLightToShader
//=============================================================================================

void LightingPass::SendDirLightToShader(DirectionalLight* dl, Scene& scene, size_t lightIndex)
{
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, scene.GetNearPlane(), scene.GetFarPlane());
	glm::mat4 lightView = glm::lookAt(dl->transform.getPosition(), dl->GetTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	//Utils::Print("Sending dirLights[" + std::to_string(lightIndex) + "]");

	lightPassShader->use();
	dl->sendToShader(
		*lightPassShader,											//shader
		lightIndex,													//light index
		scene.GetCamera()->get_view_matrix(),						//view matrix
		lightSpaceMatrix);											//light space matrix
}
