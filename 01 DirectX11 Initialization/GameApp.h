#ifndef GAMEAPP_H
#define GAMEAPP_H
#include "d3dApp.h"
#include "Graphics/TextureRender.h"
#include <memory>

#include "Scene.h"
class GameApp : public D3DApp
{
public:

	struct CB
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};


	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
	bool InitEffect();
	bool InitResource();

private:
	std::unique_ptr<TextureRender>	m_pGameContent;
	std::unique_ptr<TextureRender>	m_pRayTracingContent;
	std::shared_ptr<Scene>			m_pScene;
	
	CB m_CBuffer;	                    // 用于修改GPU常量缓冲区的变量

};


#endif