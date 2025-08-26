#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<memory>

#include"../Core/GameObject.h"
#include"../Shader.h"
#include"ShadowMap.h"

class Light : public GameObject
{
public:
	//State
	glm::vec3 color;
	bool castShadows;

	//Shadow Maps
	std::unique_ptr<ShadowMap> shadowMap = nullptr;

	//Constructors
	Light(glm::vec3 pos, bool castShadows, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

	//Destructor
	virtual ~Light() = default;

	//methods
	virtual void sendToShader(
		const Shader& shader,
		const std::string& uniformName,
		const glm::mat4& view,
		const GLint& textureUnit) const = 0;
	glm::mat4 GetModelMatrix() const = delete;	//we wont need a model matrix for a light object

	//Shadows
	ShadowMap* GetShadowMap() { return shadowMap.get(); }

};

