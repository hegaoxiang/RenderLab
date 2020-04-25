#include "GameApp.h"
#include "DXOthers/d3dUtil.h"
#include "DXOthers/DXTrace.h"
#include "GUI/GUI.h"
#include "GUI/Editor.h"
#include "Graphics/Effects.h"
#include "Logic/LogicSystem.h"
#include <IMGUI/ImGuizmo.h>
#include "Logic/CameraController.h"

using namespace DirectX;


GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance),
	m_pGameContent(new TextureRender()),
	m_pRayTracingContent(new TextureRender()),
	m_pScene(new Scene())
{
	LogicSystem::Get().SetScene(m_pScene);
}

GameApp::~GameApp()
{
	m_pScene->Serialize();
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	if (!GUI::Get().Init(m_hMainWnd, m_pd3dDevice.Get(), m_pd3dImmediateContext.Get()))
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	return true;
}

void GameApp::OnResize()
{
	if (m_pCamera != nullptr)
	{
		auto [isResize, width, height] = Editor::Get().IsGameWindowResize();

		if (isResize)
		{
			m_pCamera->SetFrustum(XM_PI / 3, width / height, 1.0f, 1000.0f);
			BasicEffect::Get().SetProjMatrix(m_pCamera->GetProjXM());

		}
	}

	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
	LogicSystem::Get().Update(dt);


	CameraController::UpdataCamera(m_pCamera.get(), m_CameraMode, dt);
	BasicEffect::Get().SetViewMatrix(m_pCamera->GetViewXM());
}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,255,255)
	

	m_pd3dImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blue);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	
	
   
	BasicEffect::Get().SetTextureDiffuse(m_pWoodCrate.Get());

	BasicEffect::Get().SetRenderDefault(m_pd3dImmediateContext.Get());

#ifdef EDITOR
	m_pGameContent->Begin(m_pd3dImmediateContext.Get(), blue);

#endif
	m_pScene->Draw(m_pd3dImmediateContext.Get(),BasicEffect::Get());


	SkyEffect::Get().SetRenderDefault(m_pd3dImmediateContext.Get());
	m_SkyRender->Draw(m_pd3dImmediateContext.Get(), SkyEffect::Get(), *m_pCamera);

#ifdef EDITOR
m_pGameContent->End(m_pd3dImmediateContext.Get());
#endif

	GUI::Get().BeginGUI();
#ifdef EDITOR
	{
		Editor::Get().OnGUI(m_pd3dDevice.Get(), m_pGameContent->GetOutputTexture(),*m_pCamera);
	}

#endif
	GUI::Get().EndGUI();

	GUI::Get().Render();

	HR(m_pSwapChain->Present(1, 0));
}


bool GameApp::InitEffect()
{
	BasicEffect::Get().InitAll(m_pd3dDevice.Get());

	SkyEffect::Get().InitAll(m_pd3dDevice.Get());

	return true;
}

bool GameApp::InitResource()
{
	Editor::Get().SetScene(m_pScene,m_pd3dDevice.Get());

	m_pGameContent->InitResource(m_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight);
	m_pRayTracingContent->InitResource(m_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight);
	
	auto camera = std::shared_ptr<FirstPersonCamera>(new FirstPersonCamera);
	m_pCamera = camera;

	camera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
	camera->LookAt(XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	// 初始化仅在窗口大小变动时修改的值
	m_pCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);

	m_SkyRender = std::make_unique<SkyRender>();
	
	HR(m_SkyRender->InitResource(m_pd3dDevice.Get(), m_pd3dImmediateContext.Get(),
		L"Texture\\daylight.jpg",
		5000.0f));
	m_SkyRender->SetDebugObjectName("Light");

	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"Texture\\floor.dds", nullptr, m_pWoodCrate.GetAddressOf()));


	BasicEffect::Get().SetProjMatrix(m_pCamera->GetProjXM());

	return true;
}

