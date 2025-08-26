#pragma once

#include"../GBuffer.h"
#include"../../Resources/Primitives/Quad.h"
#include"../../Engine/Scene.h"

//SHADER DIRECTORIES
#define LIGHTPASS_SHADER_VS "./shaders/renderer/lightingPass.vs"
#define LIGHTPASS_SHADER_FS "./shaders/renderer/lightingPass.fs"

class LightingPass
{
private:
	//Data
	unsigned int screenWidth, screenHeight;
	bool irradianceActive = false;

	//Shaders
	std::unique_ptr<Shader> lightPassShader;

	//Geometry
	Quad screenQuad;

	//Initialize
	void Initialize();

public:
	//Constructors
	LightingPass(unsigned int screenWidth, unsigned int screenHeight);

	//Render
	void Render(const Scene& scene, const GBuffer& gBuffer, const GLuint& ssaoTex);
};

