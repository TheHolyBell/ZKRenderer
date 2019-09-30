#include <d3d11.h>
#include "Engine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR szCmdLine, int)
{
	Engine engine;

	engine.Initialize(hInstance, 1280, 720, L"x64/"
#ifdef _DEBUG
		"Debug"
#else
		"Release"
#endif
		"/vertexshader.cso", 
		L"x64/"
#ifdef _DEBUG
		"Debug"
#else
		"Release"
#endif
		"/pixelshader.cso");

	while (engine.ProcessMessages())
	{
		engine.Update();
		engine.Render();
	}

	return 0;
}