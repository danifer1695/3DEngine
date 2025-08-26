#include "MaterialPBR.h"

glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 captureViews[] = {
	//all the angles at which the faces will be captured
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};
float cubeVertices[] = {
	// back face
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
	 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
	// front face
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	// left face
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	// right face
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
	 // bottom face
	 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
	  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	 // top face
	 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
	  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
};
float quadVertices[] = {
	// positions        // texture Coords
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

//===============================================================================================
//Constructors
//===============================================================================================

MaterialPBR::MaterialPBR(std::string name, TexturesPBR textures, unsigned int hdriTex)
	:Material(name), textures{ textures }, hdriTexture{ hdriTex }
{

	InitShaders();
	SetupEnvironmentCubemap();
	SetupIrradiance();
	SetupPrefiltering();
	SetupBRDFlut();
}
//===============================================================================================
//Destructor
//===============================================================================================

MaterialPBR::~MaterialPBR()
{

	//Clean up framebuffers
	glDeleteFramebuffers(1, &captureFBO);
	glDeleteRenderbuffers(1, &captureRBO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}
//===============================================================================================
//InitShaders
//===============================================================================================

void MaterialPBR::InitShaders()
{
	//Directories defined in header file

	//create PBR shaders
	this->irradianceShader = std::make_unique<Shader>("PBR_IRRADIANCE", CUBEMAP_SHADER_VS, IRRACIANCE_SHADER_FS);
	this->prefilterShader = std::make_unique<Shader>("PBR_PREFILTER", CUBEMAP_SHADER_VS, PREFILTER_SHADER_FS);
	this->hdriToCubemap = std::make_unique<Shader>("PBR_CUBEMAP", CUBEMAP_SHADER_VS, CUBEMAP_SHADER_FS);
	this->brdfShader = std::make_unique<Shader>("PBR_BRDF", BRDF_SHADER_VS, BRDF_SHADER_FS);

	//configure shader
	baseShader->use();
	baseShader->setInt("irradianceMap", 0);			//***ATTENTION TO THESE***
	baseShader->setInt("prefilterMap", 1);
	baseShader->setInt("brdfLUT", 2);
	baseShader->setInt("albedoMap", 3);
	baseShader->setInt("normalMap", 4);
	baseShader->setInt("metallicMap", 5);
	baseShader->setInt("roughnessMap", 6);
	baseShader->setInt("aoMap", 7);
}
//===============================================================================================
//SetupEnvironmentCubemap
//===============================================================================================
void MaterialPBR::SetupEnvironmentCubemap()
{
	//Setup Framebuffer
	//-----------------
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);	//contains only depth, no stencil
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	//Setup Cubemap texture
	//---------------------
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		//nullptr as data paramenter to just save memory space
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//HDR to Cubemap
	//--------------
	hdriToCubemap->use();
	hdriToCubemap->setInt("equirectangularMap", 0);
	hdriToCubemap->setMatrix4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdriTexture);

	glViewport(0, 0, 512, 512);		//configure viewport to capturing dimensions
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		//we turn the camera 6 times to capture each of the cubemap's faces from the inside
		hdriToCubemap->setMatrix4("view", captureViews[i]);
		//we tell the framebuffer which cubemap face to capture to
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//tell opengl to generate mipmaps from first mip face, to combat visible dots artifacts
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	getError("SetupEnvironmentCubemap");
}
//===============================================================================================
//SetupIrradiance()
//===============================================================================================

void MaterialPBR::SetupIrradiance()
{
	//Setup Texture and Framebuffer
	//-----------------------------
	//irradiance does not need a high definition since its just a blurred out image.
	unsigned int irradianceSize = 32;

	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, irradianceSize, irradianceSize, 0,
			GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);		//we re-use the captureFBO from earlier
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	//we change the renderbuffer's capture dimensions to those of the irradiance map texture
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, irradianceSize, irradianceSize);

	//Create the irradiance cubemap
	//-----------------------------
	irradianceShader->use();
	irradianceShader->setInt("environmentMap", 0);
	irradianceShader->setMatrix4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);		//we attach the skybox cubemap to use as base

	glViewport(0, 0, irradianceSize, irradianceSize);	//set dimensions to match irradiance map
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceShader->setMatrix4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//error check
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "ERROR: Framebuffer not complete for face " << i << std::endl;
		}

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	//reset to default framebuffer

	getError("SetupIrradiance");
}
//===============================================================================================
//SetupPrefiltering
//===============================================================================================
void MaterialPBR::SetupPrefiltering()
{
	//Setup Textures and Mipmaps
	//--------------------------
	//128x128 base mipmap resolution is usually enough for reflections
	unsigned int prefilterSize = 128;

	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, prefilterSize, prefilterSize,
			0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//Be sure to set minification filter to gl_linear_mipmap_linear to enable
	// trilinear filtering (smooth transition between mipmaps)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//we call glGenerateMipMap to allocate memory for our mipmap levels
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//Generate the Prefiltered textures
	//-------------------------------
	prefilterShader->use();
	prefilterShader->setInt("environmentMap", 0);
	prefilterShader->setMatrix4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);	//we re-use captureFBO again
	//Generate the desired number of mipmap levels (4, since the while loop runs while mip is less than 5)
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		//resize framebuffer according to mip-level size
		//Rougher (blurrier) layers will get twice as small on every step
		unsigned int mipWidth = static_cast<unsigned int>(prefilterSize * std::pow(0.5f, mip));
		unsigned int mipHeight = static_cast<unsigned int>(prefilterSize * std::pow(0.5f, mip));
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		//resize renderbuffer resolution
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterShader->setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterShader->setMatrix4("view", captureViews[i]);
			//last argument in glFramebufferTexture2D specifies the mip level we render into
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	getError("SetupPrefiltering");
}
//===============================================================================================
//SetupBRDFlut()
//===============================================================================================

void MaterialPBR::SetupBRDFlut()
{
	//Setup Texture and pre-allocate memory space
	//-------------------------------------------
	unsigned int LUTsize = 512;
	glGenTextures(1, &brdfLUT);
	glBindTexture(GL_TEXTURE_2D, brdfLUT);
	//we just need the Red and Green channels (GL_RG)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, LUTsize, LUTsize, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Reconfigure capture framebuffer
	//-------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, LUTsize, LUTsize);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);

	//Render to screen-quad
	//---------------------
	glViewport(0, 0, LUTsize, LUTsize);
	brdfShader->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	getError("SetupBRDFlut");
}
//===============================================================================================
//bind()
//===============================================================================================

void MaterialPBR::bind() const
{
	//Bind pre-computed IBL (image based lighting) data
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, brdfLUT);

	//Bind textures
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures.albedoMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures.normalMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textures.metallicMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, textures.roughnessMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, textures.aoMap);
}
//===============================================================================================
//renderCube
//===============================================================================================

void MaterialPBR::useMaterial(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& camPos)
{
	setMatrices(model, projection, view);
	baseShader->setMatrix3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
	baseShader->setVector3("camPos", camPos);
	bind();
}
//===============================================================================================
//renderCube
//===============================================================================================

void MaterialPBR::renderCube()
{
	glCullFace(GL_FRONT);
	// initialize (if necessary)
	if (cubeVAO == 0)
	{

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glCullFace(GL_BACK);
}
//===============================================================================================
//renderQuad
//===============================================================================================

void MaterialPBR::renderQuad()
{
	if (quadVAO == 0)
	{
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
//===============================================================================================
//getError
//===============================================================================================

void MaterialPBR::getError(std::string location)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "MaterialPBR::" << location << "::OpenGL error: " << err;
		if (err == 1282) std::cout << ". Check glUniforms are correctly set, with a matching type.";
		std::cout << std::endl;
	}
}