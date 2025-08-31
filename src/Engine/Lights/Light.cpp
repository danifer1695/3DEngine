#include "Light.h"

//===============================================================================================
// Constructors();
//===============================================================================================
Light::Light(LightType type, glm::vec3 pos, bool castShadows, glm::vec3 color, float intensity)
	:GameObject(pos), type{ type }, color { color }, castShadows{ castShadows }, intensity{ intensity }
{
}
