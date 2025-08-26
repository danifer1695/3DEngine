#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"../../Renderer/Shader.h"

#include<string>
#include<memory>
#include<iostream>

//SHADER DIRECTORIES
#define BASE_SHADER_VS			"./shaders/baseShader.vs"
#define BASE_SHADER_FS			"./shaders/baseShader.fs"

class Material
{
protected:
	std::string name;

	//Shader
	std::shared_ptr<Shader> baseShader;			//Display shader

public:
	//Constructor
	Material(std::string name = "default");
	Material(std::string name, const char* vshaderPath, const char* fshaderPath);

	//Destructor
	virtual ~Material();

	//Methods
	void setMatrices(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view);
	void setModel(const glm::mat4& model)			 { baseShader->use(); baseShader->setMatrix4("model", model); }
	void setProjection(const glm::mat4& projection)	 { baseShader->use(); baseShader->setMatrix4("projection", projection);}
	void setView(const glm::mat4& view)				 { baseShader->use(); baseShader->setMatrix4("view", view); }

	virtual void bind() const = 0;
	virtual void useMaterial(const glm::mat4& model,
		const glm::mat4& projection,
		const glm::mat4& view,
		const glm::vec3& camPos) = 0;
	void getError(std::string location);

	//Getters
	//-------
	std::shared_ptr<Shader> getShader() { return baseShader; }

	//NPBR getters
	virtual unsigned int getDiffuse() { return 0; };	
	virtual unsigned int getSpecular() { return 0; };
};


