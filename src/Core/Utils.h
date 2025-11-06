#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<algorithm>
#include<cmath>
#include<random>

namespace Utils
{
	//Math
	//-----------------------------------------------------------------------------------------------
	inline float Clamp(float x, float edge0, float edge1)
	{
		//if x is less than edge0, return edge0
		//if x is more than edge1, return edge1
		//otherwise return x
		return x < edge0 ? edge0 : (x > edge1 ? edge1 : x);
	}

	inline float SmoothStep(float edge0, float edge1, float x)
	{
		//takes a number 'x' in range [edge0, edge1] and translates it to range [0.0, 1.0]

		x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
		return x * x * (3.0f - 2.0f * x);
	}

	inline float Lerp(float a, float b, float t)
	{
		//smoothly interpolates between two values 'a' and 'b', based on factor 't'. 
		//'t' is in range [0.0, 1.0]
		//ex: Utils::Lerp(5.0f, 10.0f, 0.5f) returns 7.5f

		return a + t * (b - a);
	}

	inline float Randf(float min, float max)
	{
		//make the random engine static so it does not re-seed with every call
		static std::random_device rd;
		static std::mt19937 engine(rd());

		//Define range
		std::uniform_real_distribution<float> dist(min, max);

		return dist(engine);
	}

	//OpenGL
	//-----------------------------------------------------------------------------------------------
	inline void getOpenGLError(std::string location)
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << location << "::OpenGL error: " << err;
			if (err == 1280) std::cerr << " - GL_INVALID_ENUM.";
			else if (err == 1286) std::cerr << " - Invalid Framebuffer Operation.";
			else if (err == 1282) std::cerr << " - GL_INVALID_OPERATION.";
			std::cout << std::endl;
		}
	}

	/*This function takes in two framebuffers - source and desination - and copies depth data from
	source to destination*/
	inline void CopyDepthData(const GLuint& source, GLuint& destination, 
		unsigned int screenWidth, unsigned int screenHeight)
	{
		//Bind source as read, and destination as draw
		glBindFramebuffer(GL_READ_FRAMEBUFFER, source);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination);

		//Copy depth buffer
		glBlitFramebuffer(
			0, 0, screenWidth, screenHeight,
			0, 0, screenWidth, screenHeight,
			GL_DEPTH_BUFFER_BIT,
			GL_NEAREST
		);

		//Unbind framebuffers
		glBindFramebuffer(GL_FRAMEBUFFER, destination);
	}

	//Logging
	//-----------------------------------------------------------------------------------------------
	inline void Print(std::string text)
	{
		std::cout << text << std::endl;
	}
}