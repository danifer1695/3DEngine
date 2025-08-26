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
#include"../Engine/Scene.h"
#include"../Core/Utils.h"

//Renderer class knows how to render objects in the scene, manages framebuffers and render passes
class Renderer
{
private:
	unsigned int screenWidth, screenHeight;

	//Buffers
	GBuffer gBuffer;

	//Passes
	ShadowPass		shadowPass;
	GeometryPass	geometryPass;
	SSAOPass		ssaoPass;
	LightingPass	lightingPass;

	//Initialization
	void Init();

public:
	//Constructors
	Renderer(unsigned int screenWidth, unsigned int screenHeight);

	//Public methods
	void Draw(const Scene& scene);
};

