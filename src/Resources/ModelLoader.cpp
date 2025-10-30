#include "ModelLoader.h"

//===============================================================================================
// Get();
//===============================================================================================

ModelLoader& ModelLoader::Get()
{
	static ModelLoader instance;

	return instance;
}
//===============================================================================================
// loadModel();
//===============================================================================================

Handle ModelLoader::loadModel(std::string name, std::string path)
{
	Assimp::Importer importer;
	//scenes are assimp's data structure containing all the models' data
	//RadFile's first argument is the file path to load the model from.
	//Second argument are post processing options
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::string msg =  importer.GetErrorString();
		throw std::runtime_error ("ERROR::ASSIMP::" + msg);
	}
	std::string directory = path.substr(0, path.find_last_of('/')); //gets substr from char at index 0 to last '/'
	
	std::vector<Mesh> meshes;
	processNode(scene->mRootNode, scene, name, meshes, directory);

	//Import model to Resource manager, return handle
	Handle handle = ResourceManager::Get().ImportModel(name, meshes, directory);

	return handle;
}

//================================================================
// processNode
//================================================================

void ModelLoader::processNode(
	aiNode* node, 
	const aiScene* scene, 
	std::string name, 
	std::vector<Mesh>& meshes, 
	std::string path) {
	//This is a recursive function that will process the given node, and then its children, 

	//process all the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::string meshName = name + '.' + std::to_string(i);
		mesh->mName = aiString(meshName);
		meshes.push_back(processMesh(mesh, scene, path));
	}
	//do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		std::string meshName = name + '.' + std::to_string(i);
		processNode(node->mChildren[i], scene, meshName, meshes, path);
	}
}

//================================================================
// processMesh
//================================================================

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene, std::string path) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	MapPackage textures;
	ColorPackage colors;

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

		// Load all material colors
		aiColor4D diff(0.0f, 0.0f, 0.0f, 0.0f);
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff))
		{
			colors.diffuseColor = glm::vec3(diff.r, diff.g, diff.b);
		}

		aiColor4D spec(0.0f, 0.0f, 0.0f, 0.0f);
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &spec))
		{
			colors.specularColor = (spec.r + spec.g + spec.b) / 3.0f;
		}

		aiColor4D emiss(0.0f, 0.0f, 0.0f, 0.0f);
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emiss))
		{
			colors.emissiveColor = glm::vec3(emiss.r, emiss.g, emiss.b);
		}

		float gloss(0.0f);
		if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, gloss))
		{
			colors.glossinessColor = gloss;
		}

		// Load all texture types
		std::vector<std::pair<aiTextureType, TextureType>> types = {
			{ aiTextureType_DIFFUSE,	DIFFUSE },
			{ aiTextureType_SPECULAR,	SPECULAR },
			{ aiTextureType_METALNESS,	SPECULAR },		//for non-pbr metallness will act as specular
			{ aiTextureType_SHININESS,	GLOSSINESS },	
			{ aiTextureType_NORMALS,	NORMAL },
			{ aiTextureType_EMISSIVE,	EMISSIVE },
		};

		for (auto iter : types)
		{
			auto loadedTexs = LoadMaterialTextures(material, iter.first, iter.second, path);

			for (size_t i = 0; i < loadedTexs.size(); ++i)
			{
				std::string texName = loadedTexs.at(i).texPath;

				//We fill the texture map package depending on the texture type being processed
				switch (iter.second)
				{
				case DIFFUSE:
					textures.DiffuseMaps.push_back(loadedTexs.at(i).ID);
					break;
				case SPECULAR:
					textures.SpecularMaps.push_back(loadedTexs.at(i).ID);
					break;
				case NORMAL:
					textures.NormalMaps.push_back(loadedTexs.at(i).ID);
					break;
				case GLOSSINESS:
					textures.GlossinessMaps.push_back(loadedTexs.at(i).ID);
					break;
				case EMISSIVE:
					textures.EmissiveMaps.push_back(loadedTexs.at(i).ID);
					break;
				default:
					break;
				}
			}
		}

	}
	return Mesh(vertices, indices, Material(mesh->mName.C_Str(), MATERIAL_DEFAULT, textures, colors));
}
//================================================================
// loadMaterialTextures()
//================================================================

std::vector<Texture> ModelLoader::LoadMaterialTextures(
	aiMaterial* mat, 
	aiTextureType type, 
	TextureType texType,
	std::string path) {

	//loadMaterialTextures retrieves all textures of a specific type, checks if it's a repeat, 
	// puts them in a vector and returns the vector

	std::vector<Texture> toReturn;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

		aiString aiNewPath;
		mat->GetTexture(type, i, &aiNewPath);		//stores the texture's file location
		std::string newPath = aiNewPath.C_Str();	//we convert to a normal string

		//Detect if path is absolute or relative, transform to absolute if relative
		bool isAbsolute = newPath.find(':') != std::string::npos || newPath[0] == '/' || newPath[0] == '\\';
		newPath = isAbsolute ? newPath : (path + "/" + newPath);

		Handle texHandle = ResourceManager::Get().ImportTexture(newPath, texType, newPath.c_str(), false);
		toReturn.push_back(*ResourceManager::Get().GetTexture(texHandle));
	}

	//if no textures were loaded, use default texture instead
	if (toReturn.empty())
	{
		//Utils::Print("No textures loaded, using Default texture instead.");
		//toReturn.push_back(ResourceManager::Get().GetTexture("Default"));
	}
	return toReturn;
}