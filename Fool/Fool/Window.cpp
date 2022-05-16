#include "Window.h"

Window::Window(Application* app)
{
	this->App = app;
	this->hInstance = GetModuleHandle(NULL);

}

void Window::Show(bool show)
{
	ShowWindow(hwnd, show);
}

void Window::Enable(bool enable)
{
	EnableWindow(hwnd, enable);
}

void Window::MovePosition(int newXPos, int newYPos)
{
	SetWindowPos(hwnd, HWND_BOTTOM, newXPos, newYPos, 0, 0, SWP_NOSIZE);
}
