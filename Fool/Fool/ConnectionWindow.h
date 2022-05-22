#pragma once
#include "Window.h"

#define CONNECTION_WINDOW_CLASS_NAME L"CONNECTION_WINDOW_CLASS_NAME"
#define CONNECTION_WINDOW_NAME L"Подключение к серверу!"

#define CONNECTION_EDIT_PORT_HMENU 5001
#define CONNECTION_EDIT_IP_HMENU 5002
#define CONNECTION_BUTTON_HMENU 5003

#define IP_MAX_LENGTH 15

constexpr int IP_ADRESS = 16;

class Application;
class ConnectionWindow : public Window
{
public:
	HWND ipEditHWND;
	HWND portEditHWND;
	HWND buttonHWND;
public:
	ConnectionWindow(Application* app);

	bool IsValidIP(char* ip);


	static LRESULT CALLBACK ConnectionWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void RegisterWnd();
};

