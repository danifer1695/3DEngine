#pragma once

#include<memory>

#include"../../Engine/Scene.h"

#define POINT_SHADOW_SHADER_VS "./shaders/ShadowMaps/PointShadow.vs"
#define POINT_SHADOW_SHADER_GS "./shaders/ShadowMaps/PointShadow.gs"
#define POINT_SHADOW_SHADER_FS "./shaders/ShadowMaps/PointShadow.fs"

class ShadowPass
{
private:
	//Shaders
	std::unique_ptr<Shader> pointShadowShader;

	//Initialization
	void Initialize();

public:
	//Constructors
	ShadowPass();

	//Render
	void Render(const Scene& scene);
};

