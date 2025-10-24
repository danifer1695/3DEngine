#pragma once

#include<glad/glad.h> // holds all OpenGL type declarations

#include<string>

enum TextureType
{
	DEFAULT,
	DIFFUSE,
	SPECULAR,
	NORMAL,
	HEIGHT,
	EMISSIVE,
	AO,
	GLOSSINESS
};

class Texture
{
public:
	GLuint ID;
	TextureType texType;
	std::string texPath;

	//Constructors
	Texture() 
		:ID{ 0 }, texType{ DEFAULT }, texPath{ "" }{};

	Texture(GLuint id, TextureType type, std::string path)
		:ID{ id }, texType{ type }, texPath{ path } {};
};

