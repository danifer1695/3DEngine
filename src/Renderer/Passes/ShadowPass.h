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

	//Shadow Array
	GLuint dirShadowFBO;		//reusable dir shadow FBO
	GLuint pointShadowFBO;		//reusable point shadow FBO

	unsigned int DIR_SHADOW_RES = 1024;
	unsigned int DIR_SHADOW_MAX = 10;
	unsigned int POINT_SHADOW_RES = 512;
	unsigned int POINT_SHADOW_MAX = 10;

	GLuint dirShadowArray;
	GLuint pointShadowArray;

	//Initialization
	void Initialize();
	void SetupFBO();
	void SetupDirShadowArray();
	void SetupPointShadowArray();

	//Update
	void UpdateShadows(const Scene& scene, bool globalUpdate);
	void ResetDirtyFlags(const Scene& scene);
	void CaptureDirShadows(const Scene& scene, const size_t& lightIndex);
	void CapturePointShadows(const Scene& scene, const size_t& lightIndex, const glm::mat4& shadowProj);

public:
	//Constructors
	ShadowPass();

	//Render
	void Render(const Scene& scene);

	//Getters
	const GLuint GetDirTextureArray() const		{ return dirShadowArray; }
	const GLuint GetPointTextureArray() const	{ return pointShadowArray; }
};

