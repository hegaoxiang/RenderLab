#include "GRiRenderer.h"

float GRiRenderer::AspectRatio() const
{
	return static_cast<float>(mClientWidth) / mClientHeight;
}


HWND GRiRenderer::MainWnd()const
{
	return mhMainWnd;
}


void GRiRenderer::SetClientWidth(int width)
{
	mClientWidth = width;
}

void GRiRenderer::SetClientHeight(int height)
{
	mClientHeight = height;
}
void GRiRenderer::SetCamera(GRiCamera* cam)
{
	pCamera = cam;
}
void GRiRenderer::SetScene(GScene* scene)
{
	pScene = scene;
}
GRiRendererFactory* GRiRenderer::GetFactory()
{
	return mFactory.get();
}
void GRiRenderer::SyncTextures(std::unordered_map<std::string, std::unique_ptr<GRiTexture>>& mTextures)
{
	pTextures.clear();

	std::unordered_map<std::string, std::unique_ptr<GRiTexture>>::iterator i;

	for (i = mTextures.begin(); i != mTextures.end(); i++)
	{
		pTextures[i->first] = i->second.get();
	}
}
int GRiRenderer::GetClientWidth()
{
	return mClientWidth;
}

int GRiRenderer::GetClientHeight()
{
	return mClientHeight;
}
