#include "Model.h"

//================================================================
// Constructors
//================================================================

Model::Model(std::string name, std::vector<Mesh> meshes, std::string directory)
	:name{name}, directory{directory}
{
	for (auto mesh : meshes)
	{
		this->meshes.push_back(mesh);
	}
	//Utils::Print(std::to_string(this->meshes.size()));
}
//================================================================
// Draw()
//================================================================

void Model::Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model) {
	
	shader.use();
	shader.setMatrix4("model", model);
	shader.setMatrix4("projection", projection);
	shader.setMatrix4("view", view);

	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::Draw() {
	
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw();
}
//===============================================================================================
// GetMeshes();
//===============================================================================================

const std::vector<Mesh>& Model::GetMeshes() const
{
	if (meshes.size() > 0)
	{
		return meshes;
	}
	else
	{
		throw std::exception("MODEL::GET_MESHES::ERROR - No meshes found.");
	}
}
