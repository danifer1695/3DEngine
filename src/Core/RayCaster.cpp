#include "RayCaster.h"

//=============================================================================================
//IntersectsAABB
//=============================================================================================

bool RayCaster::IntersectsAABB(const Ray& ray, const AABB& aabb, float& tNear) const
{
	//slab method
	// //https://tavianator.com/2022/ray_box_boundary.html
	//Returns true if the ray intersects the AABB
	float tmin = std::numeric_limits<float>::lowest();
	float tmax = std::numeric_limits<float>::max();
	float padding = 0.01f;

	//t1 and t2 are intersection points of the ray with each of the planes
	//multiplications are faster than divisions so we invert the denominator in t = (x - origin.x)/direction.x
	float denominator = 1.0f / (ray.GetDiretion().x != 0.0f ? ray.GetDiretion().x : 1e-9f);
	float t1 = (aabb.worldMin.x - ray.GetOrigin().x) * denominator;
	float t2 = (aabb.worldMax.x - ray.GetOrigin().x) * denominator;

	tmin = std::max(tmin, std::min(t1, t2));	//intersection with the x plane
	tmax = std::min(tmax, std::max(t1, t2));
	if (tmin == tmax) { tmin -= padding; tmax += padding; }

	denominator = 1.0f / (ray.GetDiretion().y != 0.0f ? ray.GetDiretion().y : 1e-9f);
	t1 = (aabb.worldMin.y - ray.GetOrigin().y) * denominator;
	t2 = (aabb.worldMax.y - ray.GetOrigin().y) * denominator;

	tmin = std::max(tmin, std::min(t1, t2));	//intersection with the y plane
	tmax = std::min(tmax, std::max(t1, t2));
	if (tmin == tmax) { tmin -= padding; tmax += padding; }

	denominator = 1.0f / (ray.GetDiretion().z != 0.0f ? ray.GetDiretion().z : 1e-9f);
	t1 = (aabb.worldMin.z - ray.GetOrigin().z) * denominator;
	t2 = (aabb.worldMax.z - ray.GetOrigin().z) * denominator;

	tmin = std::max(tmin, std::min(t1, t2));	//intersection with the z plane
	tmax = std::min(tmax, std::max(t1, t2));
	if (tmin == tmax) { tmin -= padding; tmax += padding; }

	//reject rays that are behind the camera
	//If the farthest hit is still behind the ray origin, the ray 
	// never touches the box in front of the camera.
	if (tmax < 0.0f) return false;

	//If the entry point is behind the camera but the exit point is in front, we use tmax instead. 
	// This handles the case where the camera starts inside the AABB.
	tNear = (tmin >= 0.0f) ? tmin : tmax;

	//Utils::Print(std::to_string(tmin));
	//Utils::Print(std::to_string(tmax));

	return tmin <= tmax;
}
//=============================================================================================
//CastRay
//=============================================================================================

Item* RayCaster::CastRay(const Ray& ray, const std::map<std::string, std::unique_ptr<Item>>& items)
{
	//Cast against all objects

	Item* closest = nullptr;
	float closestT = std::numeric_limits<float>::max();

	for (auto& item : items)
	{
		float tNear;	//This variable will be modified dynamically from within IntersectAABB
		if (IntersectsAABB(ray, item.second->GetAABB(), tNear))		//Returns true if ray intersects aabb
		{
			//Utils::Print(item.second->GetName());
			//Utils::Print(std::to_string(tNear));
			if (tNear < closestT)
			{	
				closestT = tNear;
				//Utils::Print(item.second->GetName());
				closest = item.second.get();	
			}
		}
	}

	return closest;
}
