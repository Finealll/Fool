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
	void MovePositionBottom(int newXPos, int newYPos);
	void MovePositionTop(int newXPos, int newYPos);
	void TransformPosition(int angle);

	HWND GetHwnd() { return hwnd; }
};

