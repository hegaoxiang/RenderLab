#include "GameApp.h"
#include "DXOthers/d3dUtil.h"
#include "DXOthers/DXTrace.h"
#include "GUI/GUI.h"
#include "GUI/Editor.h"
#include "Graphics/Effects.h"
#include "Logic/LogicSystem.h"

using namespace DirectX;


GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance),
	m_pGameContent(new TextureRender()),
	m_pRayTracingContent(new TextureRender()),
	m_pScene(new Scene())
{
	LogicSystem::Get().SetScene(m_pScene);
	Editor::Get().SetScene(m_pScene);
}

GameApp::~GameApp()
{
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
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
	LogicSystem::Get().Update(dt);

	BasicEffect::Get().SetViewMatrix(m_CBuffer.view);
	BasicEffect::Get().SetProjMatrix(m_CBuffer.proj);
}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,255,255)
	

	m_pd3dImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blue);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	

	BasicEffect::Get().SetRenderDefault(m_pd3dImmediateContext.Get());

#ifdef EDITOR
	m_pGameContent->Begin(m_pd3dImmediateContext.Get(), blue);

#endif
	m_pScene->Draw(m_pd3dImmediateContext.Get(),BasicEffect::Get());

#ifdef EDITOR
m_pGameContent->End(m_pd3dImmediateContext.Get());
#endif

#ifdef EDITOR
	GUI::Get().BeginGUI();
	{
		Editor::Get().OnGUI(m_pd3dDevice.Get());

		ImGui::Begin("Game");
		;
		ImGui::Image(m_pGameContent->GetOutputTexture(), ImGui::GetContentRegionAvail());
		ImGui::End();

		ImGui::Begin("RayTracing");

		ImGui::End();
	}

	GUI::Get().EndGUI();

	GUI::Get().Render();
#endif

	HR(m_pSwapChain->Present(0, 0));
}


bool GameApp::InitEffect()
{
	BasicEffect::Get().InitAll(m_pd3dDevice.Get());

	return true;
}

bool GameApp::InitResource()
{
	m_pGameContent->InitResource(m_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight);
	m_pRayTracingContent->InitResource(m_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight);
	

	m_CBuffer.world = XMMatrixIdentity();	// 单位矩阵的转置是它本身
	m_CBuffer.view = (XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_CBuffer.proj = (XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));

	return true;
}
