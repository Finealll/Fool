#pragma once
#include "framework.h"
#include "Window.h"
#include  "Global.h"


#define LANDING_WINDOW_CLASS_NAME L"LANDING_WINDOW_CLASS_NAME"
#define LANDING_WINDOW_NAME L"Добро пожаловать!"

#define LANDING_LABEL_HMENU 1001
#define LANDING_EDIT_HMENU 1002
#define LANDING_BUTTON_HMENU 1003

constexpr int INPUT_NAME_LENGTH = 16;

// Окно входа
class Application;
class LoginWindow : public Window
{
private:
	HWND _labelHWND;
	HWND _editHWND;
	HWND _buttonHWND;

public:
	LoginWindow(Application* app);

	static LRESULT CALLBACK LoginWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void RegisterWnd();
};

