#pragma once
#include "../d3dUtil.h"
#include <IMGUI/imgui.h>
class GUI
{
public:
	static GUI& Get();

	bool Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	bool CallBack(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void BeginGUI();
	void EndGUI();
	void Render();

private:
	GUI();
};

