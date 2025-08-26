#include "Shader.h"

//============================================
// Constructors
//============================================

Shader::Shader(std::string name, const char* vertex_path, const char* fragment_path, const char* geometry_path) {

	Name = name;
	std::string g_shader_str = std::string(geometry_path);
	bool has_geometry_shader = (g_shader_str != "default");

	//Retrieve vertex/fragment source code from their file path
	//---------------------------------------------------------
	std::string vertex_code;
	std::string fragment_code;
	std::string geometry_code;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	std::ifstream g_shader_file;
	//Ensure ifstream can throw exceptions
	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//open files
		v_shader_file.open(vertex_path);
		f_shader_file.open(fragment_path);
		if (has_geometry_shader) g_shader_file.open(geometry_path);
		std::stringstream v_shader_stream, f_shader_stream, g_shader_stream;
		//read file's buffer contents into streams
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();
		if (has_geometry_shader) g_shader_stream << g_shader_file.rdbuf();
		//close file handlers
		v_shader_file.close();
		f_shader_file.close();
		if (has_geometry_shader) g_shader_file.close();
		//convert stream into string
		vertex_code = v_shader_stream.str();

		fragment_code = f_shader_stream.str();

		if (has_geometry_shader) geometry_code = g_shader_stream.str();
	}
	catch(std::ifstream::failure e){
		std::cout << "ERROR::SHADER::"<< Name << "::FILE_NOT_SUCCESSFULLY_READ" << e.what() << std::endl;
	}
	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();
	const char* g_shader_code = geometry_code.c_str();

	//Shader compilation
	//------------------
	unsigned int vertex;
	unsigned int fragment;
	unsigned int geometry { 0 };	//shader IDs
	int success;
	char infoLog[512];

	//vertex
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v_shader_code, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << Name << "::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f_shader_code, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << Name << "::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//geometry
	if (has_geometry_shader) {

		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &g_shader_code, NULL);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << Name << "::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	//shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (has_geometry_shader) glAttachShader(ID, geometry);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << Name << "::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//delete shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (has_geometry_shader) glDeleteShader(geometry);
}

//============================================
// use()
//============================================

void Shader::use() {
	glUseProgram(ID);
}

//============================================
// set Uniforms
//============================================

void Shader::setBool(const std::string& uniform_name, bool value) const {

	//find the uniform location in the shader
	GLint location = glGetUniformLocation(ID, uniform_name.c_str());	
	glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string& uniform_name, int value) const {

	//find the uniform location in the shader
	GLint location = glGetUniformLocation(ID, uniform_name.c_str());	
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& uniform_name, float value) const {

	GLint location = glGetUniformLocation(ID, uniform_name.c_str());	
	glUniform1f(location, value);
}

void Shader::setMatrix4(const std::string& uniform_name, glm::mat4 value) const {

	GLint location = glGetUniformLocation(ID, uniform_name.c_str());
	//second argument is the amount of matrices we want to send, 1 in this case
	//third argument ask us if we want to transpose the matrix, that is to swap columns and rows.
	//fourth argument is the actual matrix data. GLM's format doesnt always match OpenGL's expectations,
	//	so we convert the data with GLM's value_ptr function
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMatrix3(const std::string& uniform_name, glm::mat3 value) const {

	GLint location = glGetUniformLocation(ID, uniform_name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVector2(const std::string& uniform_name, float x, float y) const {

	GLint location = glGetUniformLocation(ID, uniform_name.c_str());
	glUniform2f(location, x, y);
}

void Shader::setVector2(const std::string& uniform_name, glm::vec2 vec_in) const {

	GLint location = glGetUniformLocation(ID, uniform_name.c_str());
	glUniform2f(location, vec_in.x, vec_in.y);
}

void Shader::setVector3(const std::string& uniform_name, float x, float y, float z) const {

	GLint location = glGetUniformLocation(ID, uniform_name.c_str());
	glUniform3f(location, x, y, z);
}

void Shader::setVector3(const std::string& uniform_name, glm::vec3 vec_in) const {

	GLint location = glGetUniformLocation(ID, uniform_name.c_str());
	glUniform3f(location, vec_in.x, vec_in.y, vec_in.z);
}

void Shader::setVector4(const std::string& uniform_name, float x, float y, float z) const {

	GLint location = glGetUniformLocation(ID, uniform_name.c_str());
	glUniform4f(location, x, y, z, 1.0f);
}

//============================================
// End of File
//============================================
