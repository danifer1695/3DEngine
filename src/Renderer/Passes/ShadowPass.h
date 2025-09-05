#pragma once

#include<memory>

#include"../../Engine/Scene.h"

#define POINT_SHADOW_SHADER_VS	"./shaders/ShadowMaps/PointShadow.vs"
#define POINT_SHADOW_SHADER_GS	"./shaders/ShadowMaps/PointShadow.gs"
#define POINT_SHADOW_SHADER_FS	"./shaders/ShadowMaps/PointShadow.fs"
#define DIR_SHADOW_SHADER_VS	"./shaders/ShadowMaps/DirShadow.vs"
#define DIR_SHADOW_SHADER_FS	"./shaders/ShadowMaps/DirShadow.fs"

class ShadowPass
{
private:
	//State
	bool shadowsDirty = true;

	//Shaders
	std::unique_ptr<Shader> pointShadowShader;
	std::unique_ptr<Shader> dirShadowShader;

	//Initialization
	void Initialize();

	//Update
	void UpdateShadows(const Scene& scene, bool globalUpdate);
	void ResetDirtyFlags(const Scene& scene);

public:
	//Constructors
	ShadowPass();

	//Render
	void Render(const Scene& scene);
};

