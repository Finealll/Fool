#pragma once
#include "Window.h"


#define WAITING_WINDOW_CLASS_NAME L"WAITING_WINDOW_CLASS_NAME"
#define WAITING_WINDOW_NAME L"Ожидайте..."

#define WAITING_LABEL_HMENU 2001

// Окно ожидания
class Application;
class WaitingWindow : public Window
{
private:

public:
	WaitingWindow(Application* app);

	static LRESULT CALLBACK WaitingWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void RegisterWnd();
};

