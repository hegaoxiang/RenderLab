#include "Editor.h"
#include "IMGUI/imgui_stdlib.h"
#include "IMGUI/ImGuizmo.h"



Editor& Editor::Get()
{
	static Editor* impl = new Editor();
	return *impl;
}

void Editor::OnGUI()
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
				m_pScene->AddComponent(SelectedID, (Component)k);
			else
				m_pScene->RemoveComponent(SelectedID, (Component)k);
		}
		ImGui::EndPopup();
	}
	ImGui::InputText("Name", &name);

	ImGui::End();
}

void Editor::ShowHierarchy()
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
