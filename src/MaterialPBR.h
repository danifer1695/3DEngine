#pragma once

#include"Material.h"

#define CUBEMAP_SHADER_VS		"./shaders/cubemap.vs"
#define CUBEMAP_SHADER_FS		"./shaders/cubemap.fs"
#define IRRACIANCE_SHADER_FS	"./shaders/irradiance_convolution.fs"
#define PREFILTER_SHADER_FS		"./shaders/prefilter.fs"
#define BRDF_SHADER_VS			"./shaders/brdf.vs"
#define BRDF_SHADER_FS			"./shaders/brdf.fs"

struct TexturesPBR
{
	GLuint albedoMap;			//GLuint -> OpenGL's own unsigned int variable type
	GLuint normalMap;
	GLuint metallicMap;
	GLuint roughnessMap;
	GLuint aoMap;

	TexturesPBR(GLuint albedo, GLuint normal, GLuint metallic, GLuint roughness, GLuint ao)
		:albedoMap{ albedo },
		normalMap{ normal },
		metallicMap{ metallic },
		roughnessMap{ roughness },
		aoMap{ ao } {
	};
};

class MaterialPBR : public Material
{
private:
	//framebuffers
	GLuint captureFBO, captureRBO;	//to capture hrdi to cubemap texture
	GLuint cubeVAO, cubeVBO, quadVAO, quadVBO;

	//textures
	TexturesPBR textures;

	GLuint hdriTexture;
	GLuint envCubemap;

	GLuint irradianceMap;		//stores averaged environment light efect from every angle
	GLuint prefilterMap;
	GLuint brdfLUT;				//BRDF lookup texture to calculate roughness

	//shaders
	std::unique_ptr<Shader> hdriToCubemap;			//Capture HDRi and store it in Cubemap texture
	std::unique_ptr<Shader> irradianceShader;		//Create Irradiance map out of environment map
	std::unique_ptr<Shader> brdfShader;				//Create brdf Look Up Texture (LUT)
	std::unique_ptr<Shader> prefilterShader;		//Create prefiltered textures to use to display roughness levels

	//Private Methods
	//---------------
	//Initialization
	void InitShaders();
	void SetupEnvironmentCubemap();
	void SetupIrradiance();
	void SetupPrefiltering();
	void SetupBRDFlut();

	//Helpers
	void bind() const override;

	//Primitives
	void renderCube();
	void renderQuad();

	//Debugging
	void getError(std::string location);

public:
	//Constructors
	MaterialPBR(std::string name, TexturesPBR textures, unsigned int hdriTex);

	//Delete undersired Constructors & Operators
	MaterialPBR(const MaterialPBR&) = delete;
	MaterialPBR& operator=(const MaterialPBR&) = delete;

	//Destructor
	~MaterialPBR();

	//Methods
	void useMaterial(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& camPos) override;

	//Debugging
	GLuint GetBRDFtexture() { return brdfLUT; }
};