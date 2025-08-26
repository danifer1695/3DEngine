#include "ScreenQuad.h"

//=================================================================================
// Constructors
//=================================================================================
ScreenQuad::ScreenQuad(unsigned int width, unsigned int height)
	:screenWidth{ width }, screenHeight{ height }
{
	Init();
}

//=================================================================================
// Destructor
//=================================================================================
ScreenQuad::~ScreenQuad()
{
	delete screenShader;
}

//=================================================================================
// Init
//=================================================================================

void ScreenQuad::Init()
{
	screenShader = new Shader("SCREENQUAD", SCREEN_SHADER_VS, SCREEN_SHADER_FS);

	setupFBO();
	setupTexture();

	//unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	getError("INIT");
}


//=================================================================================
// setupFBO
//=================================================================================

void ScreenQuad::setupFBO()
{
	glGenFramebuffers(1, &screenFBO);
	glGenRenderbuffers(1, &screenRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, screenRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, screenRBO);

	getError("SETUP_FBO");
}

//=================================================================================
// setupTexture
//=================================================================================

void ScreenQuad::setupTexture()
{
	glGenTextures(1, &screenTex);
	glBindTexture(GL_TEXTURE_2D, screenTex);
	//GL_RGBA16F to support HDR rendering
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	getError("SETUP_TEXTURE");
}

//=============================================================================================
//StartCapture()
//=============================================================================================

void ScreenQuad::StartCapture()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	getError("START_CAPTURE");
}

//=============================================================================================
//EndCapture()
//=============================================================================================

void ScreenQuad::EndCapture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	getError("END_CAPTURE");
}

//=============================================================================================
//RenderQuad()
//=============================================================================================

void ScreenQuad::RenderQuad()
{
	if (quadVAO == 0)
	{
		//std::cout << "Current context: " << glfwGetCurrentContext() << std::endl;
		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		getError("RENDER_QUAD::VAO_SETUP");
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	getError("RENDER_QUAD");
}

//=============================================================================================
//Draw()
//=============================================================================================

void ScreenQuad::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	getError("DRAW::SETUP");

	screenShader->use();
	screenShader->setInt("screenTex", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTex);
	getError("DRAW::TEXTURE_BIND");

	RenderQuad();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	getError("DRAW");
}

//=============================================================================================
//getError()
//=============================================================================================

void ScreenQuad::getError(std::string location)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "SCREENQUAD::" << location << "::OpenGL error: " << err;
			 if (err == 1280) std::cerr << " - GL_INVALID_ENUM.";
		else if (err == 1286) std::cerr << " - Invalid Framebuffer Operation.";
		else if (err == 1282) std::cerr << " - GL_INVALID_OPERATION.";
		std::cout << std::endl;
	}
}