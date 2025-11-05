#pragma once

#include"Handle.h"
#include"ResourceManager.h"
#include"Primitives/Quad.h"

class Icon
{
private:

	Quad quad = Quad();
	Handle h_texture;
	float scale = 0.3f;

public:

	//Constructor
	Icon();
	Icon(Handle texture);

	//Methods
	void Draw() { quad.Draw(); }

	//Getters
	const float GetScale() const { return scale; }

	//Setters
	void SetScale(float s) { scale = s; }
};

