#include "Ray.h"

//=============================================================================================
//Constructor
//=============================================================================================
Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
	:origin{origin}, direction{glm::normalize(direction)}
{
}

//=============================================================================================
//Cast
//=============================================================================================
void Ray::Cast()
{
	Utils::Print("Ray cast from: ("
		+ std::to_string(origin.x) + ", "
		+ std::to_string(origin.y) + ", "
		+ std::to_string(origin.z)
		+ ") towards: "
		+ std::to_string(direction.x) + ", "
		+ std::to_string(direction.y) + ", "
		+ std::to_string(direction.z) + ")"
	);
}
