#include "GDxRendererFactory.h"
#include "GDxTextureLoader.h"
#include "GDxMesh.h"

GDxRendererFactory::GDxRendererFactory(ID3D11Device* device)
{
	mpDevice = device;
}


GRiTextureLoader* GDxRendererFactory::CreateTextureLoader()
{
	GRiTextureLoader* texloader = new GDxTextureLoader(mpDevice);
	return texloader;
}

GRiMaterial* GDxRendererFactory::CreateMaterial()
{
	GRiMaterial* material = new GRiMaterial();
	return material;
}

GRiMesh* GDxRendererFactory::CreateMesh(std::vector<GRiMeshData> meshData)
{
	GDxMesh* mesh = new GDxMesh(mpDevice, meshData);
	return mesh;
}
