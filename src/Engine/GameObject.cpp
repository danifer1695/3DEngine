#include "GameObject.h"

//================================================================
// Constructor()
//================================================================

GameObject::GameObject(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	:name{ name }
{
	transform.SetPosition(position);
	transform.SetRotation(rotation);
	transform.SetScale(scale);
}
//================================================================
// processAABB()
//================================================================

void GameObject::InitializeAABB(std::shared_ptr<Model> model)
{
	aabb.Initialize(model);
}

