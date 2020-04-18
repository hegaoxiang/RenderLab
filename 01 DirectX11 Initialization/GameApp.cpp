#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include "GUI/GUI.h"
#include "Graphics/Effects.h"
#include "Logic/LogicSystem.h"

using namespace DirectX;

const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


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

	m_pGameContent->Begin(m_pd3dImmediateContext.Get(), blue);

	m_pScene->Draw(m_pd3dImmediateContext.Get(),BasicEffect::Get());

	m_pGameContent->End(m_pd3dImmediateContext.Get());


	GUI::Get().BeginGUI();
	{
		m_pScene->OnGUI();

		ImGui::Begin("Game");
		//ImGuiIO& io = ImGui::GetIO();
		;
		ImGui::Image(m_pGameContent->GetOutputTexture(), ImGui::GetContentRegionAvail());
		ImGui::End();

		ImGui::Begin("RayTracing");

		ImGui::End();
		ImGui::Begin("test");

		if (ImGui::Button("remove 0"))
		{
			m_pScene->RemoveComponent(0,COMPONENT_ROTATE);
		}
		if (ImGui::Button("remove 1"))
		{
			m_pScene->RemoveComponent(1, COMPONENT_ROTATE);
		}

		if (ImGui::Button("add 0"))
		{
			m_pScene->AddComponent(0, COMPONENT_ROTATE);
		}
		if (ImGui::Button("add 1"))
		{
			m_pScene->AddComponent(1, COMPONENT_ROTATE);
		}

		ImGui::End();
	}

	GUI::Get().EndGUI();

	GUI::Get().Render();

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
	

	m_pScene->CreateBox(m_pd3dDevice.Get());
	
	m_pScene->CreateBox(m_pd3dDevice.Get());

	m_pScene->AddComponent(0, COMPONENT_ROTATE);


	m_CBuffer.world = XMMatrixIdentity();	// 单位矩阵的转置是它本身
	m_CBuffer.view = (XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_CBuffer.proj = (XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));

	return true;
}
