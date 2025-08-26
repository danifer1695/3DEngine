#pragma once

#include"../GBuffer.h"
#include"../../Engine/Scene.h"

#define GEOMPASS_SHADER_VS "./shaders/renderer/geomPass.vs"
#define GEOMPASS_SHADER_FS "./shaders/renderer/geomPass.fs"

class GeometryPass
{
private:
	//Data
	unsigned int screenWidth, screenHeight;

	//Shaders
	std::unique_ptr<Shader> geomPassShader;

	//Initialization
	void Initialize();


public:
	//Constructors
	GeometryPass(unsigned int screenWidth, unsigned int screenHeight);

	//Render
	void Render(const Scene& scene, const GBuffer& gBuffer);
};

