#include "GCore.h"
#include "GCameraController.h"
#include <filesystem>
#include <string>
#include <Vendor\IMGUI\imgui_impl_win32.h>

using namespace std;
using namespace std::filesystem;
GCore::~GCore()
{
	mScene->Serialize(L"file.data");
}

GCore& GCore::GetCore()
{
	static GCore* instance = new GCore();

	return *instance;
}

void GCore::RenderFrame()
{
	Update();

	Draw();
}

void GCore::Initialize(HWND OutputWindow, double width, double height)
{
	mRenderer->PreInitialize(OutputWindow, width, height);


	mFactory = mRenderer->GetFactory();

	LoadScene();
	mRenderer->SetScene(mScene.get());

	if (!LoadAllTexture())
		return;
	mRenderer->SyncTextures(mTextures);

	LoadMaterials();
	mRenderer->SyncMaterials(mMaterials);

	LoadMeshes();
	mRenderer->SyncMeshes(mMeshes);

	LoadSceneObjects();
	mRenderer->SyncSceneObjects(mSceneObjects,mSceneObjectLayer);

	LoadCamera();
	mRenderer->SetCamera(m_pCamera.get());


	mRenderer->Initialize();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void GCore::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef USE_IMGUI
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
#endif 
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.
		// We pause the game when the window is deactivated and unpause it
		// when it becomes active.
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mAppPaused = true;
			mTimer->Stop();
		}
		else
		{
			mAppPaused = false;
			mTimer->Start();
		}
		return ;

		// WM_SIZE is sent when the user resizes the window.
	case WM_SIZE:
		// Save the new client area dimensions.
		mRenderer->SetClientWidth(LOWORD(lParam));
		mRenderer->SetClientHeight(HIWORD(lParam));
		if (mRenderer->IsRunning())
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
				{
					// If user is dragging the resize bars, we do not resize
					// the buffers here because as the user continuously
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is
					// done resizing the window and releases the resize bars, which
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return; 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing = true;
		mTimer->Stop();
		return; 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing = false;
		mTimer->Start();
		OnResize();
		return; 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return; 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses
		// a key that does not correspond to any mnemonic or accelerator key.
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return; MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return; 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			;//PostQuitMessage(0);
		}

		return; 0;
	}
}

void GCore::OnResize()
{
	mRenderer->OnResize();
}

void GCore::Update()
{
	auto& io = ImGui::GetIO();
	float dt = io.DeltaTime;

	GCameraController::UpdataCamera(m_pCamera.get(), m_CameraMode, dt);

	mRenderer->Update(dt);
}

void GCore::Draw()
{
	auto& io = ImGui::GetIO();
	float dt = io.DeltaTime;

	mRenderer->Draw(dt);
}

bool GCore::LoadAllTexture()
{
	unique_ptr<GRiTextureLoader> texloader(mFactory->CreateTextureLoader());

	path str("Texture");
	if (!exists(str))		//必须先检测目录是否存在才能使用文件入口.
		return 1;
	directory_entry entry(str);		//文件入口
	if (entry.status().type() == file_type::directory)	//这里用了C++11的强枚举类型
		;// cout << "该路径是一个目录" << endl;
	directory_iterator list(str);	        //文件入口容器

	
	int i = 0;
	for (auto& it : list)
	{
		wstring file(L"Texture\\");
		file += it.path().filename().generic_wstring();

		
		GRiTexture* tex;
		if (it.path().stem().generic_string() == "daylight")
		{

			tex = texloader->LoadTexture(file,true);
		}
		else tex = texloader->LoadTexture(file);
		tex->Name = it.path().stem().generic_string();
		//tex->UniqueFileName = (m_TexNames[i]);
		unique_ptr<GRiTexture> temp(tex);

		mTextures.insert(std::pair<string, unique_ptr<GRiTexture>>(tex->Name, move(temp)));
// 		m_TexNames.emplace_back(it.path().stem().generic_string());
// 		m_NameMap.insert(std::pair<string, int>(m_TexNames[i], i));
// 
// 		
// 		m_Textures.push_back(move(temp));
	}

	return true;
}

void GCore::LoadMaterials()
{
	auto defaultMat = std::make_unique<GRiMaterial>(*mFactory->CreateMaterial());

	defaultMat->AddTexture(mTextures["floor"].get());
	defaultMat->Name = "Default";

	mMaterials[defaultMat->Name] = move(defaultMat);


}

void GCore::LoadMeshes()
{
	unique_ptr<GRiGeometryGenerator> geo(mFactory->CreateGeometryGenerator());

	vector<GRiMeshData> meshData;
	{
		GRiMeshData cMeshData = geo->CreateBox(2, 2, 2, 0);
		meshData.push_back(cMeshData);
		auto mesh = mFactory->CreateMesh(meshData);
		mesh->UniqueName = "Box";
		mesh->Submeshes[cMeshData.MeshDataName].SetMaterial(mMaterials["Default"].get());
		unique_ptr<GRiMesh> temp1(mesh);
		mMeshes[mesh->UniqueName] = move(temp1);
	}

	meshData.clear();
	{
		GRiMeshData cMeshData = geo->CreateQuad();
		meshData.push_back(cMeshData);
		auto mesh = mFactory->CreateMesh(meshData);
		mesh->UniqueName = "Quad";
		mesh->Submeshes[cMeshData.MeshDataName].SetMaterial(mMaterials["Default"].get());
		unique_ptr<GRiMesh> temp1(mesh);
		mMeshes[mesh->UniqueName] = move(temp1);
	}

}

void GCore::LoadSceneObjects()
{
	//auto so = make_unique<GRiSceneObject>(*mFactory->CreateSceneObjet());

	unique_ptr<GRiSceneObject> so(mFactory->CreateSceneObjet());
	so->UniqueName = "TestBox";
	so->Mesh = mMeshes["Box"].get();
	XMStoreFloat4x4(&so->World,XMMatrixIdentity());
	mSceneObjects[so->UniqueName] = move(so);
	

	unique_ptr<GRiSceneObject> skySo(mFactory->CreateSceneObjet());
	skySo->UniqueName = "Sky";
	skySo->Mesh = mMeshes["Quad"].get();
	mSceneObjectLayer[(int)RenderLayer::Sky].push_back(skySo.get());
	mSceneObjects[skySo->UniqueName] = move(skySo);
}

void GCore::LoadCamera()
{
	FirstPersonCamera cam;
	//camera->SetViewPort(0.0f, 0.0f, (float)width, (float)height);

	cam.LookAt(XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	m_pCamera = std::make_unique<FirstPersonCamera>(cam);


	// 初始化仅在窗口大小变动时修改的值
	m_pCamera->SetFrustum(XM_PI / 3, mRenderer->AspectRatio(), 0.5f, 1000.0f);

}

void GCore::LoadScene()
{
	mScene->AntiSerialize(L"file.data");
}

GCore::GCore()
{
	mTimer = std::make_unique<GRiTimer>();

	mRenderer = &GDxRenderer::GetRenderer();

	mScene = std::make_unique<GScene>();
}
