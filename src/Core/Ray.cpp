#include "Ray.h"

//=============================================================================================
//Constructor
//=============================================================================================
Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
	:origin{origin}, direction{glm::normalize(direction)}
{
}


