#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<memory>

#include"../GameObject.h"
#include"../../Renderer/Shader.h"
#include"ShadowMap.h"

enum LightType
{
	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	SPOTLIGHT
};

class Light : public GameObject
{
protected:

public:
	//State
	bool active = true;

	LightType type;
	glm::vec3 color;
	float intensity;

	//Shadows
	bool castShadows;
	bool softShadows = true;

	//Shadow Maps

	//Constructors
	Light(std::string name,
		LightType type, 
		glm::vec3 pos, 
		bool castShadows, 
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), 
		float intensity = 1.0f);

	//Destructor
	virtual ~Light() = default;

	//methods
	glm::mat4 GetModelMatrix() const = delete;	//we wont need a model matrix for a light object

	//Getters
	const LightType		GetLightType() const	{ return type; }
	const bool			GetCastShadows() const	{ return castShadows; }
	const bool			GetSoftShadows() const	{ return softShadows; }

	//Setters
	void SetIntensity(const float& i)		{ intensity = i; }
	void SetCastShadows(const float& cast)	{ castShadows = cast;			transform.SetIsDirty(true); }
	void ToggleCastShadows()				{ castShadows = !castShadows;	transform.SetIsDirty(true); }
	void SetSoftShadows(const float& soft)	{ softShadows = soft;			transform.SetIsDirty(true); }
	void ToggleSoftShadows()				{ softShadows = !softShadows;	transform.SetIsDirty(true); }
};

