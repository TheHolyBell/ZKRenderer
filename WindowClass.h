#pragma once
#include <Windows.h>
#include <string>
#include "Graphics.h"

class Window
{
public:
	void InitAndCreateWindow(HINSTANCE hInstance, int Width, int Height, LPCTSTR szWindowTitle, DWORD style);
	virtual LRESULT m_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND GetWindowHandler();

	int GetWidth();
	int GetHeight();

	std::wstring GetWindowName();
	std::wstring GetWindowClassName();

	int GetMousePosX();
	int GetMousePosY();
	POINT GetMousePos();

private:
	bool CreateWindowClass(LPCTSTR szWindowTitle, LPCTSTR szWindowClass);

protected:

	HWND m_hwnd;
	HINSTANCE m_hInstance;

	std::wstring m_windowName;
	std::wstring m_windowClassName;
	int m_Width;
	int m_Height;

	Graphics m_gfx;

	POINT m_mousePos = {};
};