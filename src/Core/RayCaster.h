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

	template<typename T>
	std::shared_ptr<T> CastRay(const Ray& ray, std::vector<std::shared_ptr<T>>& items)
	{
		//Make sure incoming type inherits from GameObject
		static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");

		//Cast against all objects
		std::shared_ptr<T> closest = nullptr;
		float closestT = std::numeric_limits<float>::max();

		for (auto& item : items)
		{
			float tNear;	//This variable will be modified dynamically from within IntersectAABB
			if (IntersectsAABB(ray, item->GetAABB(), tNear))		//Returns true if ray intersects aabb
			{
				if (tNear < closestT)
				{
					closestT = tNear;
					closest = item;
				}
			}
		}

		return closest;
	}
};

