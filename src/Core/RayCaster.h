#pragma once

#include <map>

#include"Ray.h"
#include"../Engine/Item.h"
#include"../Engine/Collisions/AABB.h"

class RayCaster
{
private:

	bool IntersectsAABB(const Ray& ray, const AABB& aabb, float& tNear) const;

public:
	RayCaster() {};
	Item* CastRay(const Ray& ray, const std::map<std::string, std::unique_ptr<Item>>& items);
};

