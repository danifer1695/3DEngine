#pragma once

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader
{
public:
	unsigned int ID;	//The shader program's ID
	std::string Name;

	Shader(std::string name, const char* vertex_path, const char* fragment_path) 
		: Shader(name, vertex_path, fragment_path, "default") {};		//No geometry shader constructor
	Shader(std::string name, const char* vertex_path, const char* fragment_path, const char* geometry_path);

	void use();		//Activate shader

	void setBool(const std::string& uniform_name, bool value) const;
	void setInt(const std::string& uniform_name, int value) const;
	void setFloat(const std::string& uniform_name, float value) const;
	void setMatrix4(const std::string& uniform_name, glm::mat4 value) const;
	void setMatrix3(const std::string& uniform_name, glm::mat3 value) const;
	void setVector2(const std::string& uniform_name, float x, float y) const;
	void setVector2(const std::string& uniform_name, glm::vec2 vec_in) const;			//overloaded to take in a vec3
	void setVector3(const std::string& uniform_name, float x, float y, float z) const;
	void setVector3(const std::string& uniform_name, glm::vec3 vec_in) const;			//overloaded to take in a vec3
	void setVector4(const std::string& uniform_name, float x, float y, float z) const;
};

