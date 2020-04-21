#ifndef GAMEAPP_H
#define GAMEAPP_H
#include "d3dApp.h"
#include "Graphics/TextureRender.h"
#include <memory>
#include "Logic/Camera.h"
#include "Scene.h"

namespace RenderContext
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// Direct3D 11
	static ComPtr<ID3D11Device> pd3dDevice;                    // D3D11设备
	static ComPtr<ID3D11DeviceContext> pd3dImmediateContext;   // D3D11设备上下文
	static ComPtr<IDXGISwapChain> pSwapChain;                  // D3D11交换链
}
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
	
	// 摄像机模式
	enum class CameraMode { FirstPerson, ThirdPerson, Free };

	std::shared_ptr<Camera>			m_pCamera;
	CameraMode m_CameraMode;			// 摄像机模式

	CB m_CBuffer;	                    // 用于修改GPU常量缓冲区的变量

};


#endif