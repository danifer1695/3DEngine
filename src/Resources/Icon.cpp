#include "Icon.h"

//=============================================================================================
//Constructors()
//=============================================================================================

Icon::Icon()
{
	h_texture = ResourceManager::Get().GetTextureHandle("Light_Icon");
}

Icon::Icon(Handle texture)
{
	h_texture = texture;
}
