#include "GDxRendererFactory.h"
#include "GDxTextureLoader.h"

GDxRendererFactory::GDxRendererFactory(ID3D11Device* device)
{
	mpDevice = device;
}


GRiTextureLoader* GDxRendererFactory::CreateTextureLoader()
{
	GRiTextureLoader* texloader = new GDxTextureLoader(mpDevice);
	return texloader;
}
