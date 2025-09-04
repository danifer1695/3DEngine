#include "Light.h"

//===============================================================================================
// Constructors();
//===============================================================================================
Light::Light(std::string name, LightType type, glm::vec3 pos, bool castShadows, glm::vec3 color, float intensity)
	:GameObject(name, pos), type{ type }, color { color }, castShadows{ castShadows }, intensity{ intensity }
{
}
