#pragma once
#include "framework.h"


// ������� ����� ��� ����
class Application;
class Window
{
protected:
	HINSTANCE hInstance;
	HWND hwnd;
public:
	Application* App;

	Window(Application* app);

	void Show(bool show = true);
	void Enable(bool enable = true);
	void MovePosition(int newXPos, int newYPos);

	HWND GetHwnd() { return hwnd; }
};

