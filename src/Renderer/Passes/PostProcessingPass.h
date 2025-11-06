#pragma once

#include<glad/glad.h>

#include"../Shader.h"
#include"../../Engine/Scene.h"
#include"../../Core/Utils.h"
#include"../../Resources/ResourceManager.h"
#include"../../Resources/Primitives/Quad.h"
#include"../../Renderer/GBuffer.h"

#define POST_SHADER_VS "./shaders/renderer/postProcessingPass.vs"
#define POST_SHADER_FS "./shaders/renderer/postProcessingPass.fs"
#define OUTLINE_SHADER_VS "./shaders/renderer/outlineShader.vs"
#define OUTLINE_SHADER_FS "./shaders/renderer/outlineShader.fs"
#define ICON_SHADER_VS "./shaders/renderer/iconShader.vs"
#define ICON_SHADER_FS "./shaders/renderer/iconShader.fs"

class PostProcessingPass
{
private:
	//Data
	unsigned int screenWidth, screenHeight;

	GLuint FBO, RBO;
	GLuint texture;

	std::unique_ptr<Shader> postProcessingShader;
	std::unique_ptr<Shader> outlineShader;
	std::unique_ptr<Shader> iconShader;

	Quad screenQuad;

	//Icons
	bool renderIcons = true;
	float iconSize = 0.2f;

	//Handles
	Handle h_lightIconTexture;

	//Initialization
	void SetupFBO();
	void SetupTexture();

	//Helpers
	void RenderItemSelection(Scene& scene);
	void RenderIcons(Scene& scene, const GBuffer& gbuffer);

public:
	//Constructor
	PostProcessingPass(unsigned int screenWidth, unsigned int screenHeight);

	//UI
	glm::vec3 selectionColor = glm::vec3(0.9f, 0.9f, 0.1f);
	float selectionThickness = 3.0f;

	//Render
	void Render(Scene& scene, GLuint renderTex, const GBuffer& gbuffer);
	void ToggleIcons() { renderIcons = !renderIcons; }

	//Getters
	const GLuint	GetTexture() const		{ return texture; }
	const bool		GetRenderIcons() const	{ return renderIcons; }
	const float		GetIconSize() const		{ return iconSize; }

	//Setters
	void SetRenderIcons(bool set)	{ renderIcons = set; }
	void SetIconSize(float set)		{ iconSize = set; }
};

