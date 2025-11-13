#pragma once

#include<iostream>
#include<string>

#include<glm/glm.hpp>

#include "../../Resources/Model.h"
#include "../../Core/Utils.h"

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
	void Initialize(const Model* model);
	void InitializeWithCube(const std::vector<glm::vec3>& verts);
	void ComputeLocalCorners();
	void Update(glm::mat4 modelMatrix);
};

