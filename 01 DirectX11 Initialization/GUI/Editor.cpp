#include "Editor.h"
#include <IMGUI/imgui_stdlib.h>
#include <IMGUI/ImGuizmo.h>

Editor& Editor::Get()
{
	static Editor* impl = new Editor();
	return *impl;
}

void Editor::OnGUI(ID3D11Device* device)
{
	// need to choose a scene first 
	if (!m_pScene)
		return;

	if (m_ShowHierarchy)
		ShowHierarchy();

	if (m_ShowInspector)
		ShowInspector();
	if (HasComponent(COMPONENT_TRANSFORM))
		ShowTransForm();


}

void Editor::ShowInspector()
{
	ImGui::Begin("Inspector");

	if (SelectedID == -1)
	{
		ImGui::End();
		return;
	}

	auto& name = m_pScene->names[SelectedID];
	auto& mask = m_pScene->masks[SelectedID];

	bool toggles[] = { mask & COMPONENT_TRANSFORM,mask&COMPONENT_MODEL, mask & COMPONENT_ROTATE };

	if (ImGui::Button("Components"))
	{
		ImGui::OpenPopup("_Comp");
	}
	if (ImGui::BeginPopup("_Comp"))
	{
		UINT totalComp = NameMap.size();

		UINT k = 1;
		for (UINT i = 0,j = k; i < totalComp; i++)
		{
			j = k << i;
			ImGui::MenuItem(NameMap[(Component)j].c_str(), "", &toggles[i]);
		}

		k = 1;
		for (UINT i = 0, j = k; i < totalComp; i++)
		{
			j =k << i;
			if (toggles[i])
				AddComponent((Component)j);
			else
				RemoveComponent((Component)j);
		}
		ImGui::EndPopup();
	}
	ImGui::InputText("Name", &name);

	ImGui::End();
}

void Editor::ShowHierarchy()
{
	auto& names = m_pScene->names;

	ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("Box"))
			{
				CreateBox();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	UINT i = 0;
	for (auto& item : names)
	{
		if (ImGui::Selectable(item.c_str(), SelectedID == i))
			SelectedID = i;
		i++;
	}

	ImGui::End();
}

void Editor::ShowTransForm()
{
	auto& worldMats = m_pScene->worldMats;

	ImGui::Begin("TransForm");

	ImGui::Text("transform");
	ImGui::NewLine();

	static float translation[3];
	static float rotation[3];
	static float scale[3];

	float mat[16];
	ImGuizmo::DecomposeMatrixToComponents(worldMats[SelectedID].data(), translation, rotation, scale);
	ImGui::InputFloat3("Tr", translation, 3);
	ImGui::InputFloat3("Rt", rotation, 3);
	ImGui::InputFloat3("Sc", scale, 3);
	ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, mat);

	for (size_t i = 0; i < 16; i++)
	{
		worldMats[SelectedID][i] = mat[i];
	}

	ImGui::End();
}

void Editor::ShowModel()
{

}

void Editor::InitAdditionData()
{
	for (size_t i = 0; i < m_pScene->Num; i++)
	{
		if (m_pScene->masks[i] & COMPONENT_MODEL)
		{
			SelectedID = i;
			PrimaryModel p = (PrimaryModel)m_pScene->modelType[i];
			switch (p)
			{
			case (PrimaryModel::BOX):
			{
				SetMesh(m_pd3dDevice.Get(), Geometry::CreateBox<VertexPosColor>());
			}
			break;
			default:
				break;
			}
		}
	}
}

void Editor::SetMesh( ID3D11Device* device, const void* vertices, UINT vertexSize, UINT vertexCount, const void* indices, UINT indexCount, DXGI_FORMAT indexFormat)
{
	HR(SelectedID != -1);
	auto& m_Model = m_pScene->modelParts[SelectedID];
	m_Model.vertexStride = vertexSize;

	m_Model.vertexCount = vertexCount;
	m_Model.indexCount = indexCount;
	m_Model.indexFormat = indexFormat;

	m_Model.vertexBuffer.Create(device, vertices, vertexSize, vertexCount);

	m_Model.indexBuffer.Create(device, indices, indexCount, indexFormat);
}

void Editor::CreateEnity()
{
	for (size_t i = 0; i < m_pScene->Num; i++)
	{
		if (m_pScene->masks[i] == COMPONENT_NONE)
		{
			SelectedID = i;
			return;
		}
	}
}

void Editor::CreateBox()
{
	CreateEnity();

	// Name
	m_pScene->names[SelectedID] = "testBox" ;
	// Transform
	AddComponent(COMPONENT_TRANSFORM);

	m_pScene->worldMats[SelectedID][0] = 1;
	m_pScene->worldMats[SelectedID][5] = 1;
	m_pScene->worldMats[SelectedID][10] = 1;
	m_pScene->worldMats[SelectedID][15] = 1;
	// Model
	AddComponent(COMPONENT_MODEL);
	m_pScene->modelType[SelectedID]=(UINT)PrimaryModel::BOX;

	static int q = 0;
	if (q == 2)
		m_pScene->Serialize();
	if (q == 0)
	{
		q = 1;
		SetMesh(m_pd3dDevice.Get(), Geometry::CreateBox<VertexPosColor>());
	}
	else
	{
		q = 2;
		std::vector<VertexPosColor>vertices =
		{
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
		};

		std::vector<DWORD> indices = {
			// 正面
			0, 1, 2,
			2, 3, 0,
			// 左面
			4, 5, 1,
			1, 0, 4,
			// 顶面
			1, 5, 6,
			6, 2, 1,
			// 背面
			7, 6, 5,
			5, 4, 7,
			// 右面
			3, 2, 6,
			6, 7, 3,
			// 底面
			4, 0, 3,
			3, 7, 4
		};
		SetMesh(m_pd3dDevice.Get(), vertices, indices);

	}
}
