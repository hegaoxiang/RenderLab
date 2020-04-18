#ifndef GAMEAPP_H
#define GAMEAPP_H
#include "d3dApp.h"
#include "TextureRender.h"
#include <memory>
#include "GameObject.h"
#include "Scene.h"
class GameApp : public D3DApp
{
public:

	struct VertexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	};

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
	std::unique_ptr<Scene>			m_pScene;
	
	CB m_CBuffer;	                    // 用于修改GPU常量缓冲区的变量

};


#endif