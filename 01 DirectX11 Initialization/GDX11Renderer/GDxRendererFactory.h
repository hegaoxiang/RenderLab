#pragma once
#include "DXOthers/d3dUtil.h"
#include <wrl/client.h>
#include "GRendererInfra/GRiRendererFactory.h"

class GDxRendererFactory:public GRiRendererFactory
{

public:
	GDxRendererFactory(ID3D11Device* device);

	virtual GRiTextureLoader* CreateTextureLoader() override;


	virtual GRiMaterial* CreateMaterial() override;


	virtual GRiMesh* CreateMesh(std::vector<GRiMeshData> meshData) override;

private:

	ID3D11Device* mpDevice;
};

