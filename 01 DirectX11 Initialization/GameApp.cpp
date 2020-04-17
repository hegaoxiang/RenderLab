#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include "GUI/GUI.h"

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance),
	m_pGameContent(new TextureRender())
{
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

	m_pGameContent->InitResource(m_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight);


	return true;

}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
	
}

void GameApp::DrawScene()
{
	static float f[3] = {};

	GUI::Get().BeginGUI();
	{

		static bool show_demo_window = false;
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&f); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


	}

	GUI::Get().EndGUI();
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,255,255)
	

	m_pd3dImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blue);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	m_pGameContent->Begin(m_pd3dImmediateContext.Get(), blue);

	//
	m_pGameContent->End(m_pd3dImmediateContext.Get());

	GUI::Get().Render();

	HR(m_pSwapChain->Present(1, 0));
}
