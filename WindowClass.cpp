#include "WindowClass.h"

Window* g_window = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return g_window->m_WndProc(hwnd, msg, wParam, lParam);
}

bool Window::CreateWindowClass(LPCTSTR szWindowTitle, LPCTSTR szWindowClass)
{
	g_window = this;

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = szWindowClass;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	bool success = RegisterClassEx(&wc);

	return success;
}

void Window::InitAndCreateWindow(HINSTANCE hInstance, int Width, int Height, LPCTSTR szWindowTitle, DWORD style)
{
	m_hInstance = hInstance;
	m_Width = Width;
	m_Height = Height;
	m_windowName = szWindowTitle;
	m_windowClassName = L"ZKEngineWindowClass";

	bool bCreating = CreateWindowClass(m_windowName.c_str(), m_windowClassName.c_str());
	if (!bCreating)
		return;

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - Width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - Height / 2;

	RECT rc = {};
	rc.left = centerScreenX;
	rc.top = centerScreenY;
	rc.right = Width + rc.left;
	rc.bottom = Height + rc.top;

	AdjustWindowRect(&rc, style, false);

	m_hwnd = CreateWindow(m_windowClassName.c_str(), m_windowName.c_str(), style, rc.left, rc.top, m_Width, m_Height,
		nullptr, nullptr, m_hInstance, nullptr);

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
	SetForegroundWindow(m_hwnd);
}

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT Window::m_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
	{
		int newWidth = LOWORD(lParam);
		int newHeight = HIWORD(lParam);
		m_gfx.OnResize(newWidth, newHeight);
		break;
	};

	case WM_MOUSEMOVE:
		m_mousePos.x = LOWORD(lParam);
		m_mousePos.y = HIWORD(lParam);
		break;

	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		m_mousePos.x = LOWORD(lParam);
		m_mousePos.y = HIWORD(lParam);
		break;

	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		m_mousePos.x = LOWORD(lParam);
		m_mousePos.y = HIWORD(lParam);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND Window::GetWindowHandler()
{
	return m_hwnd;
}

int Window::GetWidth()
{
	return m_Width;
}

int Window::GetHeight()
{
	return m_Height;
}

std::wstring Window::GetWindowName()
{
	return m_windowName;
}

std::wstring Window::GetWindowClassName()
{
	return m_windowClassName;
}

int Window::GetMousePosX()
{
	return m_mousePos.x;
}

int Window::GetMousePosY()
{
	return m_mousePos.y;
}

POINT Window::GetMousePos()
{
	return m_mousePos;
}
