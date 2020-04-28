#pragma once
#include "Scene.h"
#include <unordered_map>
#include <vector>
#include <functional>

// Not only Editor Scene,but also Init scene.
class Camera;
class Editor
{
	using CompName = std::string;

	std::unordered_map<Component, CompName> NameMap;
	
public:
	static Editor& Get();

	void OnGUI(ID3D11Device* device, ID3D11ShaderResourceView* gameContent, const Camera& camera);

	// Also init some Other Helpful Data
	void SetScene(std::shared_ptr<Scene> scene,ID3D11Device * device)
	{
		m_pScene = scene;
		m_pd3dDevice = device;
		m_pScene->AntiSerialize();
		InitAdditionData();
	}

	// use for game window size change
	bool IsGameWindowResize();
	std::tuple<float, float> GetWindowSize();

	// changing selected one
	void RayCheck(const Camera& camera);
protected:
	void ShowInspector();

	void ShowHierarchy();
	void ShowTransForm();
	void ShowModel();

	void ShowGame(ID3D11ShaderResourceView* gameContent, const Camera& camera);
	void OnAddComponent(Component c);
	void InitModelData();
private:
	//////////////////////////////////////////////////////////////////////////
	// data help function
	//////////////////////////////////////////////////////////////////////////

	// merely init vertexbuffer now
	void InitAdditionData();

	void SetMesh(ID3D11Device* device, const void* vertices, UINT vertexSize, UINT vertexCount,
		const void* indices, UINT indexCount, DXGI_FORMAT indexFormat);

	template<class VertexType, class IndexType>
	void SetMesh(ID3D11Device* device, const std::vector<VertexType>& vertices, const std::vector<IndexType>& indices)
	{
		static_assert(sizeof(IndexType) == 2 || sizeof(IndexType) == 4, "The size of IndexType must be 2 bytes or 4 bytes!");
		static_assert(std::is_unsigned<IndexType>::value, "IndexType must be unsigned integer!");

		SetMesh(device,
			vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
			indices.data(), (UINT)indices.size(),
			(sizeof(IndexType) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT);
	}

	template<class VertexType, class IndexType>
	void SetMesh(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData)
	{
		SetMesh( device, meshData.vertexVec, meshData.indexVec);
	}


	bool HasComponent(Component c) {
		if (SelectedID != -1)
		{
			if (m_pScene->masks[SelectedID] & c)
				return true;
		}
		return false;
	}


	void AddComponent(Component c) { 
		HR(SelectedID != -1);
		m_pScene->masks[SelectedID] |= c;

		OnAddComponent(c);
	};
	void RemoveComponent(Component c) { 
		HR(SelectedID != -1);
		m_pScene->masks[SelectedID] &= ~c;
	}

	void CreateEnity();
	void CreateOBJ(Shape type);


	// Edited one  
	UINT SelectedID = -1;

	// use for ray check
	std::tuple<float, float> m_StartPos;

	bool m_ShowInspector = true;
	bool m_ShowHierarchy = true;
	bool m_ShowSetting = true;


	std::shared_ptr<Scene> m_pScene;
	std::shared_ptr<Camera>m_pCamera;
private:
	Editor() {
		NameMap = { {COMPONENT_TRANSFORM,"Transform"},{COMPONENT_MODEL,"Model"},{COMPONENT_ROTATE,"Rotate"} };
	};


	
	bool m_GWSizeChange = false;
	std::tuple<float, float> m_GameSize;

	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// Direct3D 11
	ComPtr<ID3D11Device> m_pd3dDevice;                    // D3D11…Ë±∏
};

