#include "Light.h"

//===============================================================================================
// Constructors();
//===============================================================================================
Light::Light(glm::vec3 pos, bool castShadows, glm::vec3 color)
	:GameObject(pos), color{color}, castShadows{castShadows}
{
}
