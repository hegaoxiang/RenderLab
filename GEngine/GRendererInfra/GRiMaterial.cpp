#include "GRiMaterial.h"

void GRiMaterial::AddTexture(GRiTexture* tex)
{
	pTextures.push_back(tex->Name);
}

void GRiMaterial::AddTexture(std::string tex)
{
	pTextures.push_back(tex);
}
