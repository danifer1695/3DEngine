#pragma once

#include<iostream>

#include<glm/glm.hpp>

#include "../../Resources/Model.h"

class AABB
{
public:
	glm::vec3 min;
	glm::vec3 max;

	glm::vec3 worldMin = glm::vec3(0.0f);
	glm::vec3 worldMax = glm::vec3(0.0f);

	glm::vec3 localCorners[8];

	//Constructors
	AABB() : min(0.0f), max(0.0f) { ComputeLocalCorners(); }
	AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) 
	{
		ComputeLocalCorners();
	}
	
	//Methods
	void Initialize(std::shared_ptr<Model> model);
	void ComputeLocalCorners();
	void Update(glm::mat4 modelMatrix);
};

