#include "Scene.h"

void Scene::Add(std::shared_ptr<GameObject> item)
{
	m_AllItem.push_back(item);
}

void Scene::OnGUI()
{
	ImGui::Begin("Hierarchy");
	int i = 0;
	for (auto& item : m_AllItem)
	{
		if (ImGui::Selectable(item->Name.c_str(), SelectedID == i))
			SelectedID = i;
		i++;
	}
	ImGui::End();
}

void Scene::Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect)
{
	for (auto&item :m_AllItem)
	{
		item->Draw(deviceContext, effect);
	}
}
