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

	void OnGUI();

	void SetScene(shared_ptr<Scene> scene)
	{
		m_pScene = scene;
	}

protected:
	void ShowInspector();

	void ShowHierarchy();
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
	shared_ptr<Scene> m_pScene;
private:
	Editor() {
		NameMap = { {COMPONENT_TRANSFORM,"Transform"},{COMPONENT_ROTATE,"Rotate"} };
	};


	bool m_ShowInspector = true;

	bool m_ShowHierarchy = true;
/*	bool m_ShowTransform = false;*/
};

