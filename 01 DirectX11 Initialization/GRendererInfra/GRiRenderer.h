#pragma once
#include <windows.h>
#include <memory>
#include "GRiTimer.h"
#include "GRiRendererFactory.h"
#include "GRiCamera.h"
#include <GEngine/GScene.h>

class GRiRenderer
{
public:
	virtual void Initialize() = 0;

	virtual void PreInitialize(HWND OutputWindow, double width, double height) = 0;

	virtual void Update(const float gt) = 0;

	virtual void Draw(const float gt) = 0;

	virtual void OnResize() = 0;


	virtual float AspectRatio() const;
	HWND MainWnd() const;

	int GetClientWidth();
	int GetClientHeight();
	void SetClientWidth(int width);
	void SetClientHeight(int height);

	void SetCamera(GRiCamera* cam);
	void SetScene(GScene* scene);
	virtual void CreateRendererFactory() = 0;
	GRiRendererFactory* GetFactory();

	GRiCamera* pCamera = nullptr;

	GScene* pScene = nullptr;

	
protected:
	HWND    mhMainWnd = nullptr;

	int mClientWidth = 800;
	int mClientHeight = 600;

	std::unique_ptr<GRiRendererFactory> mFactory;

public:

	virtual void SyncTextures(std::unordered_map<std::string, std::unique_ptr<GRiTexture>>& mTextures);

	std::unordered_map<std::string, GRiTexture* > pTextures;

	//GRiTimer* mTimer;
};

