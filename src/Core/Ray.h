#pragma once

#include<glm/glm.hpp>

#include<string>

#include"Utils.h"

struct RaycastHit
{
	bool hit = false;
	float distance = 0.0f;
	glm::vec3 point;
	glm::vec3 normal;
	float objectID = -1.0f;
};

class Ray
{
private:
	glm::vec3 origin;
	glm::vec3 direction;

public:
	//Constructor
	Ray(const glm::vec3& origin, const glm::vec3& direction);

	//Methods
	void Cast();

	//Getters
	const glm::vec3 GetOrigin() const		{ return origin; }
	const glm::vec3 GetDiretion() const		{ return direction; }
};

