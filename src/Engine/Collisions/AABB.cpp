#include "AABB.h"

//===============================================================================================
//Initialize()
//===============================================================================================

void AABB::Initialize(std::shared_ptr<Model> model)
{
	glm::vec3 tempMin(std::numeric_limits<float>::max());
	glm::vec3 tempMax(std::numeric_limits<float>::lowest());

	for (const auto& m : model->GetMeshes())
	{
		for (const auto& v : m.GetVertexPositions())
		{
			//std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
			//Compare each of the vertices in all meshes to min and max to get the bounding edges
			tempMin = glm::min(tempMin, v);
			tempMax = glm::max(tempMax, v);
		}
	}

	this->min = tempMin;
	this->max = tempMax;

	ComputeLocalCorners();

	/*std::cout << "AABB initialized - Min: (" << 
		this->min.x << ", " << this->min.y << ", " << this->min.z << "), Max: (" <<
		this->max.x << ", " << this->max.y << ", " << this->max.z << ")." << std::endl;*/
}
//===============================================================================================
//Update()
//===============================================================================================

void AABB::ComputeLocalCorners()
{
	localCorners[0] = { min.x, min.y, min.z };
	localCorners[1] = { max.x, min.y, min.z };
	localCorners[2] = { min.x, max.y, min.z };
	localCorners[3] = { max.x, max.y, min.z };
	localCorners[4] = { min.x, min.y, max.z };
	localCorners[5] = { max.x, min.y, max.z };
	localCorners[6] = { min.x, max.y, max.z };
	localCorners[7] = { max.x, max.y, max.z };
}
//===============================================================================================
//Update()
//===============================================================================================

void AABB::Update(glm::mat4 modelMatrix)
{
	//We update all 8 corners of the box
	glm::vec3 newMin(std::numeric_limits<float>::max());
	glm::vec3 newMax(std::numeric_limits<float>::lowest());

	for (int i = 0; i < 8; i++)
	{
		glm::vec3 worldPos = glm::vec3(modelMatrix * glm::vec4(localCorners[i], 1.0f));
		newMin = glm::min(newMin, worldPos);
		newMax = glm::max(newMax, worldPos);
	}

	this->worldMin = newMin;
	this->worldMax = newMax;

	/*std::cout << "AABB updated - Min: (" <<
		this->worldMin.x << ", " << this->worldMin.y << ", " << this->worldMin.z << "), Max: (" <<
		this->worldMax.x << ", " << this->worldMax.y << ", " << this->worldMax.z << ")." << std::endl;*/
}
