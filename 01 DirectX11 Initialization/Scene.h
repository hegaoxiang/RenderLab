#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "GUI/GUI.h"

class Scene:public IDisplay
{
public:
	auto begin()
	{
		return m_AllItem.begin();
	}
	auto end()
	{
		return m_AllItem.end();
	}

	void Add(std::shared_ptr<GameObject> item);

	virtual void OnGUI() override;

	void Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect);

	UINT SelectedID = -1;
public:
	std::vector<std::shared_ptr<GameObject>> m_AllItem;
};

