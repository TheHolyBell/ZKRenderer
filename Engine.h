#pragma once
#include "WindowClass.h"
#include "Graphics.h"
#include "Camera.h"
#include "Timer.h"

class Engine : Window
{
	Timer timer;
public:
	void Initialize(HINSTANCE hInstance, int Width, int Height, std::wstring vertexShaderPath, std::wstring pixelShaderPath, bool realTimeCompiling = false);
	void Render();
	void Update();
	bool ProcessMessages();
};