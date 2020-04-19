#pragma once
#include "Scene.h"
#include <unordered_map>
#include <vector>
class Editor
{
	using CompName = std::string;

	std::unordered_map<Component, CompName> NameMap;
	
public:
	static Editor& Get();

	void OnGUI(ID3D11Device* device);

	void SetScene(shared_ptr<Scene> scene)
	{
		m_pScene = scene;
	}

protected:
	void ShowInspector();

	void ShowHierarchy(ID3D11Device* device);
	void ShowTransForm();
	void ShowModel();
private:
	// data help function
	bool HasComponent(Component c) {
		if (m_pScene->SelectedID != -1)
		{
			if (m_pScene->masks[m_pScene->SelectedID] & c)
				return true;
		}
		return false;
	}


	void AddComponent(Component c) { 
		auto& id = m_pScene->SelectedID;
		HR(id != -1);
		m_pScene->masks[id] |= c;
	};
	void RemoveComponent(Component c) { 
		auto& id = m_pScene->SelectedID;
		HR(id != -1);
		m_pScene->masks[id] &= ~c;
	}

	void CreateEnity();
	void CreateBox(ID3D11Device* device);
	shared_ptr<Scene> m_pScene;
private:
	Editor() {
		NameMap = { {COMPONENT_TRANSFORM,"Transform"},{COMPONENT_ROTATE,"Rotate"} };
	};


	bool m_ShowInspector = true;

	bool m_ShowHierarchy = true;
/*	bool m_ShowTransform = false;*/
};

