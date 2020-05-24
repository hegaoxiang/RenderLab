#include "pch.h"
#include "Core.h"

DLLEXPORT void __stdcall InitD3D(HWND hWnd, double width, double height)
{
	//GCore::GetCore().Initialize(hWnd, width, height);
}

DLLEXPORT int __stdcall Run(void)
{
	//GCore::GetCore().Run();
	while (true)
	{
	}
	return 1;
}

DLLEXPORT void __stdcall MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return;
}
