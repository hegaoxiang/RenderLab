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
		ShowHierarchy(device);

	if (m_ShowInspector)
		ShowInspector();
	if (HasComponent(COMPONENT_TRANSFORM))
		ShowTransForm();


}

void Editor::ShowInspector()
{
	auto& SelectedID = m_pScene->SelectedID;

	ImGui::Begin("Inspector");

	if (SelectedID == -1)
	{
		ImGui::End();
		return;
	}

	auto& name = m_pScene->names[SelectedID];
	auto& mask = m_pScene->masks[SelectedID];

	bool toggles[] = { mask & COMPONENT_TRANSFORM, mask & COMPONENT_ROTATE };

	if (ImGui::Button("Components"))
	{
		ImGui::OpenPopup("_Comp");
	}
	if (ImGui::BeginPopup("_Comp"))
	{
		UINT totalComp = NameMap.size();
		

		UINT k = 1;
		for (UINT i = 0; i < totalComp; i++)
		{
			k = k << i;
			ImGui::MenuItem(NameMap[(Component)k].c_str(), "", &toggles[i]);
		}

		k = 1;
		for (UINT i = 0; i < totalComp; i++)
		{
			k <<= i;
			if (toggles[i])
				AddComponent((Component)k);
			else
				RemoveComponent((Component)k);
		}
		ImGui::EndPopup();
	}
	ImGui::InputText("Name", &name);

	ImGui::End();
}

void Editor::ShowHierarchy(ID3D11Device* device)
{
	auto& SelectedID = m_pScene->SelectedID;
	auto& names = m_pScene->names;

	ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("Box"))
			{
				CreateBox(device);
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
	auto& SelectedID = m_pScene->SelectedID;

	ImGui::Begin("TransForm");

	ImGui::Text("transform");
	ImGui::NewLine();

	static float translation[3];
	static float rotation[3];
	static float scale[3];

	ImGuizmo::DecomposeMatrixToComponents((float*)worldMats[SelectedID].m, translation, rotation, scale);
	ImGui::InputFloat3("Tr", translation, 3);
	ImGui::InputFloat3("Rt", rotation, 3);
	ImGui::InputFloat3("Sc", scale, 3);
	ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, (float*)worldMats[SelectedID].m);

	ImGui::End();
}

void Editor::ShowModel()
{

}

void Editor::CreateEnity()
{
	for (size_t i = 0; i < m_pScene->Num; i++)
	{
		if (m_pScene->masks[i] == COMPONENT_NONE)
		{
			m_pScene->SelectedID = i;
			return;
		}
	}
}

void Editor::CreateBox(ID3D11Device* device)
{
	CreateEnity();

	auto i = m_pScene->SelectedID;
	m_pScene->names[i] = "test" + i;

	XMStoreFloat4x4(&m_pScene->worldMats[i], XMMatrixIdentity());

	m_pScene->modelParts[i].primary = Geometry::BOX;

	static int q = 0;
	if (q == 2)
		m_pScene->Serialize();
	if (q == 0)
	{
		q = 1;
		m_pScene->SetMesh(i, device, Geometry::CreateBox<VertexPosColor>());
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
		m_pScene->SetMesh(i, device, vertices, indices);
		//AddComponent(i, COMPONENT_ROTATE);
	}

	AddComponent(COMPONENT_TRANSFORM);
}
