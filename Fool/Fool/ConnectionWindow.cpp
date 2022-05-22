#include "ConnectionWindow.h"

#include <regex>

#include "Application.h"

ConnectionWindow::ConnectionWindow(Application* app) : Window(app)
{
    hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CONNECTION_WINDOW_CLASS_NAME,                     // Window class
        CONNECTION_WINDOW_NAME,    // Window text
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        static_cast<LPVOID>(this)
    );

    ipEditHWND = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_TABSTOP | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT,

        20, 60, 140, 20,
        hwnd,
        (HMENU)CONNECTION_EDIT_IP_HMENU,
        hInstance,
        0
    );

    portEditHWND = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_TABSTOP | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT,

        160, 60, 100, 20,
        hwnd,
        (HMENU)CONNECTION_EDIT_PORT_HMENU,
        hInstance,
        0
    );

    buttonHWND = CreateWindowEx(
        0,
        L"BUTTON",
        L"Начать игру!",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 

        20, 90, 240, 50,
        hwnd,
        (HMENU)CONNECTION_BUTTON_HMENU,
        hInstance,
        0
    );
    SendMessage(ipEditHWND, WM_SETTEXT, 0, (LPARAM)L"192.168.56.1");
    SendMessage(portEditHWND, WM_SETTEXT, 0, (LPARAM)L"9021");


    SendMessage(ipEditHWND, EM_SETLIMITTEXT, INPUT_NAME_LENGTH, 0);
}

bool ConnectionWindow::IsValidIP(char* ip)
{
    static const std::regex rej("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\.|$)){4}");
    return std::regex_match(ip, rej);
}

LRESULT CALLBACK ConnectionWindow::ConnectionWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static ConnectionWindow* connection = nullptr;

    switch (uMsg)
    {
    case WM_CREATE:
        connection = (ConnectionWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);      //Id окна, вызвавшее событие
        int wmEvent = HIWORD(wParam);       //Id события
        if (wmId == CONNECTION_BUTTON_HMENU)
        {
            char ip[IP_MAX_LENGTH];
            GetWindowTextA(connection->ipEditHWND, ip, IP_MAX_LENGTH);
            char portStr[100];
            GetWindowTextA(connection->portEditHWND, portStr, 100);
            int port = atoi(portStr);

	        if (!connection->IsValidIP(ip))
	        {
	            MessageBox(hwnd, L"Введите корректный IP!", LB_OKAY, 0);
	        }
	        else if(port == 0)
            {
                MessageBox(hwnd, L"Введите корректный порт!", LB_OKAY, 0);
            } else
            {
                connection->App->MakeFoolClient(ip, port);
                if (connection->App->GetClient() != nullptr)
                    connection->App->Login();
            }
        }
        return 1;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT     ps01;
        HDC             hdc01;
        BITMAP          bitmap01;
        HDC             hdcMem01;
        HGDIOBJ         oldBitmap01;

        HBITMAP hBitmap01 = (HBITMAP)LoadImage(NULL, L"background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


        hdc01 = BeginPaint(hwnd, &ps01);

        hdcMem01 = CreateCompatibleDC(hdc01);
        oldBitmap01 = SelectObject(hdcMem01, hBitmap01);

        GetObject(hBitmap01, sizeof(bitmap01), &bitmap01);
        BitBlt(hdc01, 0, 0, bitmap01.bmWidth, bitmap01.bmHeight, hdcMem01, 0, 0, SRCCOPY);

        SelectObject(hdcMem01, oldBitmap01);
        DeleteDC(hdcMem01);

        //PAINTSTRUCT ps;
        HFONT hFont, hFont1;
        //HDC hdc;


        hFont = CreateFont(48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");

        SelectObject(hdc01, hFont);
        SetTextColor(hdc01, RGB(255, 255, 255));
        SetBkMode(hdc01, 1);
        TextOut(hdc01, 20, 10, L"IP", wcslen(L"IP"));

        SelectObject(hdc01, hFont);
        TextOut(hdc01, 160, 10, L"Port", wcslen(L"Port"));
        //EndPaint(hwnd, &ps01);

        EndPaint(hwnd, &ps01);
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ConnectionWindow::RegisterWnd()
{
    WNDCLASS wc = { };

    wc.lpfnWndProc = ConnectionWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CONNECTION_WINDOW_CLASS_NAME;

    RegisterClass(&wc);
}
