#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<memory>
#include<map>

#include"GBuffer.h"
#include"Passes/GeometryPass.h"
#include"Passes/ShadowPass.h"
#include"Passes/SSAO.h"
#include"Passes/LightingPass.h"
#include"Passes/IDPass.h"
#include"Passes/PostProcessingPass.h"
#include"../Engine/Scene.h"
#include"../Core/Utils.h"


enum RenderState
{
	RENDER_LIGHT,
	RENDER_ID
};

//Renderer class knows how to render objects in the scene, manages framebuffers and render passes
class Renderer
{
private:
	unsigned int screenWidth, screenHeight;

	//State
	RenderState state = RENDER_LIGHT;

	//Buffers
	GBuffer gBuffer;
	GLuint renderFBO;
	GLuint renderRBO;

	//Textures
	GLuint renderTex;

	//Passes
	ShadowPass			shadowPass;
	GeometryPass		geometryPass;
	SSAOPass			ssaoPass;
	LightingPass		lightingPass;
	IDPass				idPass;
	PostProcessingPass	postPass;

	//Initialization
	void Init();
	void SetupFBO();

	//Helpers
	void RenderSkybox(Scene& scene, const GLuint& targetFBO);
	void CopyDepthData();

public:
	//Constructors
	Renderer(unsigned int screenWidth, unsigned int screenHeight);

	//Public methods
	void Draw(Scene& scene);

	//Setters
	void				SetSelectColor(glm::vec3 color)			{ postPass.selectionColor = color; }
	void				SetSelectThickness(float size)			{ postPass.selectionThickness = size; }
	void				SetRenderState(RenderState newState)	{ state = newState; }

	//Getters
	const GLuint		GetTexture() const			{ return renderTex; }
	SSAOPass&			GetSSAOPass()				{ return ssaoPass; }
	const IDPass&		GetIDPass() const			{ return idPass; }
	const GLuint		GetPostProcessed() const	{ return postPass.GetTexture(); }
	const glm::vec3		GetSelectColor() const		{ return postPass.selectionColor; }	//pass by value
	const float			GetSelectThicness() const	{ return postPass.selectionThickness; }	//pass by value
};

