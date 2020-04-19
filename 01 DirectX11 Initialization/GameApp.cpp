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


	auto cam1st = std::dynamic_pointer_cast<FirstPersonCamera>(m_pCamera);
	auto cam3rd = std::dynamic_pointer_cast<ThirdPersonCamera>(m_pCamera);


	auto& io = ImGui::GetIO();
	if (io.MouseDown[1])
	{
		// 第一人称/自由摄像机的操作
		
		// 方向移动
		if (ImGui::IsKeyDown(Keys::W))
		{
			if (m_CameraMode == CameraMode::FirstPerson)
				cam1st->Walk(dt * 3.0f);
			else
				cam1st->MoveForward(dt * 3.0f);
		}
		if (ImGui::IsKeyDown(Keys::S))
		{
			if (m_CameraMode == CameraMode::FirstPerson)
				cam1st->Walk(dt * -3.0f);
			else
				cam1st->MoveForward(dt * -3.0f);
		}
		if (ImGui::IsKeyDown(Keys::A))
			cam1st->Strafe(dt * -3.0f);
		if (ImGui::IsKeyDown(Keys::D))
			cam1st->Strafe(dt * 3.0f);

		// 将位置限制在[-8.9f, 8.9f]的区域内
		// 不允许穿地
		XMFLOAT3 adjustedPos;
		XMStoreFloat3(&adjustedPos, XMVectorClamp(cam1st->GetPositionXM(), XMVectorSet(-8.9f, 0.0f, -8.9f, 0.0f), XMVectorReplicate(8.9f)));
		cam1st->SetPosition(adjustedPos);

		// 视野旋转，防止开始的差值过大导致的突然旋转
		cam1st->Pitch(io.MouseDelta.y * dt * 1.25f);
		cam1st->RotateY(io.MouseDelta.x * dt * 1.25f);
	}
// 	else if (m_CameraMode == CameraMode::ThirdPerson)
// 	{
// 		// 第三人称摄像机的操作
// 
// 		cam3rd->SetTarget(m_WoodCrate.GetPosition());
// 
// 		// 绕物体旋转
// 		cam3rd->RotateX(mouseState.y * dt * 1.25f);
// 		cam3rd->RotateY(mouseState.x * dt * 1.25f);
// 		cam3rd->Approach(-mouseState.scrollWheelValue / 120 * 1.0f);
// 	}

	// 更新观察矩阵
	m_pCamera->UpdateViewMatrix();
/*	XMStoreFloat4(&m_CBFrame.eyePos, m_pCamera->GetPositionXM());*/
	m_CBuffer.view = (m_pCamera->GetViewXM());

// 	// 重置滚轮值
// 	m_pMouse->ResetScrollWheelValue();
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

	HR(m_pSwapChain->Present(1, 0));
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
	
	auto camera = std::shared_ptr<FirstPersonCamera>(new FirstPersonCamera);
	m_pCamera = camera;

	camera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
	camera->LookAt(XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	// 初始化仅在窗口大小变动时修改的值
	m_pCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);


	m_CBuffer.proj = camera->GetProjXM();

	return true;
}
