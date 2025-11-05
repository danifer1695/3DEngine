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
	GLuint FBO, RBO;
	GLuint screenTex;

	//State
	bool irradianceActive = false;
	const int maxPointLights = 5;
	const int maxDirLights = 5;
	const int maxSpotLights = 5;

	//Shaders
	std::unique_ptr<Shader> lightPassShader;

	//Geometry
	Quad screenQuad;

	//Initialize
	void Initialize();
	void SetupFBO();
	void SetupDirShadowArray();

	//Helpers
	void SendPointLightToShader(PointLight* pl, Scene& scene, size_t lightIndex);
	void SendDirLightToShader(DirectionalLight* dl, Scene& scene, size_t lightIndex);

public:
	//Constructors
	LightingPass(unsigned int screenWidth, unsigned int screenHeight);

	//Render
	void Render(
		Scene& scene, 
		const GBuffer& gBuffer, 
		const GLuint& ssaoTex, 
		const bool& ssaoEnabled, 
		const GLuint& dirShadowArray, 
		const GLuint& pointShadowArray, 
		const GLuint& targetFBO);

	//Getters
	const GLuint& GetTexture() { return screenTex; }
	GLuint& GetFBO() { return FBO; }
};

