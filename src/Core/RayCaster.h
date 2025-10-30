#pragma once

#include <map>

#include"Ray.h"
#include"../Engine/Item.h"
#include"../Engine/Collisions/AABB.h"
#include"../Resources/ResourceManager.h"

class RayCaster
{
private:

	bool IntersectsAABB(const Ray& ray, const AABB& aabb, float& tNear) const;

public:
	RayCaster() {};
	Item* CastRay(const Ray& ray, std::vector<Item>& items);
};

