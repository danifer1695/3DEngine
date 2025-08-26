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

void LightingPass::Render(const Scene& scene, const GBuffer& gBuffer, const GLuint& ssaoTex)
{
	size_t numberOfLights = scene.GetLightCollection().size();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	lightPassShader->use();
	lightPassShader->setInt("numberOfLights", numberOfLights);
	lightPassShader->setFloat("farPlane", scene.GetFarPlane());
	lightPassShader->setFloat("materialShininess", 16.0f);
	lightPassShader->setMatrix4("inverseViewMatrix", glm::inverse(scene.GetCamera()->get_view_matrix()));
	Utils::getOpenGLError("LIGHTINGPASS::LIGHTING_PASS::UNIFORM_SETUP");

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
	Utils::getOpenGLError("LIGHTINGPASS::LIGHTING_PASS::TEX_BINDING");

	//send light info to shader
	for (size_t i = 0; i < numberOfLights; ++i)
	{
		//start index at GL_TEXTURE5
		int texUnitIndex = 5 + i;
		//conditional: if GL_TEXTURE0 + texUnitIndex exceeds GL_TEXTURE31, break the loop
		if (texUnitIndex > 31) break;
		GLint texUnit = GL_TEXTURE0 + texUnitIndex;

		if (auto* pl = dynamic_cast<PointLight*>(scene.GetLightCollection()[i].get()))
		{

			pl->sendToShader(
				*lightPassShader,								//shader
				"pointLights[" + std::to_string(i) + "]",		//uniform name
				scene.GetCamera()->get_view_matrix(),			//view matrix
				texUnitIndex);									//texture unit

			//Bind ShadowMap Texture
			pl->GetShadowMap()->BindTexture(texUnit);
		}
	}

	screenQuad.Draw();

	Utils::getOpenGLError("LIGHTINGPASS::LIGHTING_PASS");
}