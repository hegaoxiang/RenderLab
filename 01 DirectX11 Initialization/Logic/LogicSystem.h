#pragma once
#include "Scene.h"

class LogicSystem
{
public:
	static LogicSystem& Get();

	void Update(float dt);

	void SetScene(shared_ptr<Scene> scene)
	{
		m_pScene = scene;
	}
private:
	shared_ptr<Scene> m_pScene;
	void RotateComp(float dt);
	LogicSystem() {};

};

