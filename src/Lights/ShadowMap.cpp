#include "ShadowMap.h"

//============================================================================
//Constructors
//============================================================================
ShadowMap::ShadowMap(ShadowType type, unsigned int width, unsigned int height)
	:type{type}, width{width}, height{height}
{
	Init(width, height);
}

//============================================================================
//Destructor
//============================================================================

ShadowMap::~ShadowMap()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &depthMap);
}

//============================================================================
//Init()
//============================================================================

void ShadowMap::Init(unsigned int width, unsigned int height)
{
	//depth framebuffer
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &depthMap);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (type == Point)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
		//we reserve memory for all 6 textures of the cubemap
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width,
				height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
		
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		Utils::getOpenGLError("SHADOWMAP::INIT::POINT");
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		Utils::getOpenGLError("SHADOWMAP::INIT::NON_POINT");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//check the framebuffer was set up correctly
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	Utils::getOpenGLError("SHADOWMAP::INIT");
}

//============================================================================
//BeginCapture()
//============================================================================

void ShadowMap::BeginCapture()
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);		//we render to the depthFBO
	glClear(GL_DEPTH_BUFFER_BIT);

	Utils::getOpenGLError("SHADOWMAP::BEGIN_CAPTURE");
}

//============================================================================
//EndCapture()
//============================================================================

void ShadowMap::EndCapture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Utils::getOpenGLError("SHADOWMAP::END_CAPTURE");
}

//============================================================================
//BindTexture()
//============================================================================

void ShadowMap::BindTexture(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	if (type == Point)
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	else
		glBindTexture(GL_TEXTURE_2D, depthMap);

	Utils::getOpenGLError("SHADOWMAP::TEXTURE_BIND");
}