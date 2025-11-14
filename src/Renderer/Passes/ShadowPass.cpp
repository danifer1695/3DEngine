#include "ShadowPass.h"

//=============================================================================================
//Constructors
//=============================================================================================

ShadowPass::ShadowPass()
{
	Initialize();
	SetupFBO();
	SetupDirShadowArray();
	SetupPointShadowArray();
}
//=============================================================================================
//Initialize
//=============================================================================================

void ShadowPass::Initialize()
{
	//Build and compile shaders
	pointShadowShader = std::make_unique<Shader>("RENDERER::POINT_SHADOW", POINT_SHADOW_SHADER_VS, POINT_SHADOW_SHADER_FS);
	dirShadowShader = std::make_unique<Shader>("RENDERER::DIRECTIONAL_SHADOW", DIR_SHADOW_SHADER_VS, DIR_SHADOW_SHADER_FS);
}
//=============================================================================================
//SetupFBO
//=============================================================================================

void ShadowPass::SetupFBO()
{
	glGenFramebuffers(1, &dirShadowFBO);
	glGenFramebuffers(1, &pointShadowFBO);
}
//=============================================================================================
//SetupDirShadowArray
//=============================================================================================

void ShadowPass::SetupDirShadowArray()
{
	glGenTextures(1, &dirShadowArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, dirShadowArray);

	glTexImage3D(
		GL_TEXTURE_2D_ARRAY,
		0,
		GL_DEPTH_COMPONENT32F,
		DIR_SHADOW_RES,			//width
		DIR_SHADOW_RES,			//height
		DIR_SHADOW_MAX,			//number of layers
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr);

	//texture parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//white beyond edges of the texture
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
//=============================================================================================
//SetupPointShadowArray
//=============================================================================================

void ShadowPass::SetupPointShadowArray()
{
	glGenTextures(1, &pointShadowArray);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, pointShadowArray);

	glTexImage3D(
		GL_TEXTURE_CUBE_MAP_ARRAY,
		0,
		GL_DEPTH_COMPONENT32F,
		POINT_SHADOW_RES,
		POINT_SHADOW_RES,
		6 * POINT_SHADOW_MAX,		//6 faces per cube
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr);					//we dont add any data yet

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
}
//=============================================================================================
//Render
//=============================================================================================

void ShadowPass::Render(Scene& scene)
{
	bool needsUpdate = false;
	bool globalUpdate = false;

	//Check Items
	for (const auto& item : scene.GetGameObjectCollection())
	{
		if (item->transform.GetIsDirty())
		{
			needsUpdate = true;
			globalUpdate = true;
			break;
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

void ShadowPass::ResetDirtyFlags(Scene& scene)
{
	//Reset dirty flags
	for (auto& obj : scene.GetGameObjectCollection())
	{
		obj->transform.SetIsDirty(false);
	}
}
//=============================================================================================
//UpdateShadows
//=============================================================================================

void ShadowPass::UpdateShadows(Scene& scene, bool globalUpdate)
{
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, scene.GetNearPlane(), scene.GetFarPlane());
	size_t numberOfPointLights = scene.GetPointLightCount();
	size_t numberOfDirLights = scene.GetDirLightCount();

	//Light type indexes
	size_t dirLightIndex = 0;
	size_t pointLightIndex = 0;

	for (auto& obj : scene.GetGameObjectCollection())
	{
		//Dynamic cast to only to make sure we are working with the light type we want
		if (auto pl = std::dynamic_pointer_cast<PointLight>(obj))
		{
			if (!pl->GetCastShadows()) continue;
			if (!globalUpdate && !pl->transform.GetIsDirty()) continue;

			CapturePointShadows(scene, pointLightIndex, pl, shadowProj);
			pointLightIndex++;
			continue;
		}
		if (auto dl = std::dynamic_pointer_cast<DirectionalLight>(obj))
		{
			if (!dl->GetCastShadows()) continue;
			if (!globalUpdate && !dl->transform.GetIsDirty()) continue;

			CaptureDirShadows(scene, dirLightIndex, dl);
			dirLightIndex++;
			continue;
		}
	}
}
//=============================================================================================
//CaptureDirShadows
//=============================================================================================

void ShadowPass::CaptureDirShadows(Scene& scene, const size_t& lightIndex, std::shared_ptr<DirectionalLight> dl)
{
	glm::vec3 lightPos = dl->transform.getPosition();
	glm::vec3 lightTarget = dl->GetTarget();

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, scene.GetNearPlane(), scene.GetFarPlane());
	glm::mat4 lightView = glm::lookAt(lightPos, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	dirShadowShader->use();
	dirShadowShader->setMatrix4("lightSpaceMatrix", lightSpaceMatrix);

	//Setup FBO and Texture for capture
	glBindFramebuffer(GL_FRAMEBUFFER, dirShadowFBO);

	glFramebufferTextureLayer(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		dirShadowArray,
		0,									//mip level
		(GLint)lightIndex						//layer - we pass the DIRECTIONAL LIGHT INDEX, we only want dir lights here
	);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glViewport(0, 0, DIR_SHADOW_RES, DIR_SHADOW_RES);
	glClear(GL_DEPTH_BUFFER_BIT);

	//Draw all items in the scene
	std::cout << "Rendering Direct Shadows" << std::endl;
	for (auto& obj : scene.GetGameObjectCollection()) {
		if(auto item = std::dynamic_pointer_cast<Item>(obj))
		{
			dirShadowShader->setMatrix4("model", item->transform.GetModelMatrix());
			ResourceManager::Get().GetModel(item->getModelHandle())->Draw();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//=============================================================================================
//CapturePointShadows
//=============================================================================================

void ShadowPass::CapturePointShadows(Scene& scene, const size_t& lightIndex, std::shared_ptr<PointLight> pl, const glm::mat4& shadowProj)
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

	std::cout << "Rendering Point Shadows" << std::endl;
	//Draw all faces
	for (int face = 0; face < 6; ++face)
	{
		size_t layer = (lightIndex * 6) + face;

		//Bind framebuffer and setup capture texture
		glBindFramebuffer(GL_FRAMEBUFFER, pointShadowFBO);
		glFramebufferTextureLayer(
			GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			pointShadowArray,
			0,	//mipmap level
			(GLint)layer
			);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glViewport(0, 0, POINT_SHADOW_RES, POINT_SHADOW_RES);
		glClear(GL_DEPTH_BUFFER_BIT);

		pointShadowShader->use();
		pointShadowShader->setFloat("farPlane", scene.GetFarPlane());
		pointShadowShader->setVector3("lightPos", lightPos);
		pointShadowShader->setMatrix4("shadowMatrix", shadowTransforms[face]);
		//pointShadowShader->setInt("lightIndex", lightIndex);

		//Render all elements in the scene
		for (auto& obj : scene.GetGameObjectCollection()) 
		{
			if(auto item = std::dynamic_pointer_cast<Item>(obj))
			{
				//Send matrices to shader and draw model
				pointShadowShader->setMatrix4("model", item->transform.GetModelMatrix());
				ResourceManager::Get().GetModel(item->getModelHandle())->Draw();
			}
		}
	}
	//Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
