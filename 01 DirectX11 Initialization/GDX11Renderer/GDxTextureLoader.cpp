#include "GDxTextureLoader.h"
#include "DXOthers/DXTrace.h"
#include "DXOthers/DDSTextureLoader.h"
#include "GDxTexture.h"
#include <filesystem>
#include <string>
using namespace std;
using namespace std::filesystem;

using namespace DirectX;
using namespace Microsoft::WRL;

GRiTexture* GDxTextureLoader::LoadTexture(std::wstring fileName)
{
	GDxTexture* tex = new GDxTexture();
	
	// only dds has the s in the last location
	if (fileName[fileName.size() - 1] == 's')
	{
		HR(CreateDDSTextureFromFile(mpDevice, fileName.c_str(), nullptr, tex->mSRV.GetAddressOf()));
	}
	else
	{
		HR(CreateWICTextureFromFile(mpDevice, fileName.c_str(), nullptr, tex->mSRV.GetAddressOf()));
	}
	return tex;
}
