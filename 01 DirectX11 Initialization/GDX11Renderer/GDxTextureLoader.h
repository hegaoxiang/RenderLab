#pragma once
#include "GRendererInfra/GRiTextureLoader.h"
#include <d3d11.h>

class GDxTextureLoader :
	public GRiTextureLoader
{
public:
	GDxTextureLoader(ID3D11Device* device)
		:mpDevice(device) {};

	virtual GRiTexture* LoadTexture(std::wstring fileName) override;
private:

	ID3D11Device* mpDevice;
};

