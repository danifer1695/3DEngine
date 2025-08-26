#include "Model.h"

//================================================================
// Constructors
//================================================================

Model::Model(const char* path)
{
	loadModel(path);
}

//================================================================
// Draw()
//================================================================

void Model::Draw()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw();
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::Draw(std::shared_ptr<Shader> shader) {

	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model) {
	
	shader.use();
	shader.setMatrix4("model", model);
	shader.setMatrix4("projection", projection);
	shader.setMatrix4("view", view);

	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

//================================================================
// loadModel()
//================================================================

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	//scenes are assimp's data structure containing all the models' data
	//RadFile's first argument is the file path to load the model from.
	//Second argument are post processing options
	const aiScene* scene = importer.ReadFile(path, 
		aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/')); //gets substr from char at index 0 to last '/'

	processNode(scene->mRootNode, scene);
}

//================================================================
// processNode
//================================================================

void Model::processNode(aiNode* node, const aiScene* scene) {
	//This is a recursive function that will process the given node, and then its children, 
	//	and then that children's children, and so on.

	//process all the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	//do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

//================================================================
// processMesh
//================================================================

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		
		//Process position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		//Process Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		//Process Tangents
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		//Process TexCoord
		//We check if the mesh has texture coordinates
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			//Assimp allows for every vertex to have multiple texture coordinates (up to 8)
			//but for now we will just worry about the first one (mTextureCoords[0])
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}


		vertices.push_back(vertex);
	}
	
	//we process the indices, which are stored in the mesh's faces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//meshes contain indexes to material objects. to get the material, we need to index the scene's 
	//mMaterials array. We access the index through mMaterialIndex
	if (mesh->mMaterialIndex >= 0) {
		//we retrieve the aiMaterial object from the scene's aiMaterials array
		//a material object internally stores  an array of texture locations for each texture type.
		//different texture types are prefixed by "aiTextureType_"
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps  = loadMaterialTextures(material, aiTextureType_DIFFUSE,  "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> normalMaps   = loadMaterialTextures(material, aiTextureType_HEIGHT,   "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	}
	return Mesh(vertices, indices, textures);
}

//================================================================
// loadMaterialTextures()
//================================================================

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {

	//loadMaterialTextures retrieves, loads and initializes the textures from the material.
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

		aiString str;
		mat->GetTexture(type, i, &str);		//stores the texture's file location

		//we compare the incoming texture to already stored textures to see if we need it load it
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			//if the incoming file path is the same as any of the stored texture file paths
			
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {

				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			//if texture hasn't been loaded already, load it;
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}

	}

	return textures;
}

//================================================================
// TextureFromFile()
//================================================================

unsigned int Model::TextureFromFile(const char* path, const std::string& directory, bool gamma) {

	//Uses stbi to get the image data from its file location

	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	std::cout << "Trying to load texture: " << filename << std::endl;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else {
			std::cerr << "Unsupported number of components: " << nrComponents << std::endl;
			stbi_image_free(data);
			return 0;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

//================================================================
// ()
//================================================================

