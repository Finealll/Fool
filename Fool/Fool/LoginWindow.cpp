#include "LoginWindow.h"

#include "Application.h"

LoginWindow::LoginWindow(Application* app) : Window(app)
{
    hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        LANDING_WINDOW_CLASS_NAME,                     // Window class
        LANDING_WINDOW_NAME,    // Window text
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        static_cast<LPVOID>(this)
    );

    //_labelHWND = CreateWindowEx(
    //    0,
    //    L"STATIC",
    //    L"Введите никнейм: ",
    //    WS_TABSTOP | WS_CHILD | WS_VISIBLE | SS_CENTER,
    //    20, 20, 240, 30,
    //    hwnd,
    //    (HMENU)LANDING_LABEL_HMENU,
    //    hInstance,
    //    0
    //);

    _editHWND = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_TABSTOP | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT,

        60, 60, 200, 20,
        hwnd,
        (HMENU)LANDING_EDIT_HMENU,
        hInstance,
        0
    );

    _buttonHWND = CreateWindowEx(
        0,
        L"BUTTON",
        L"Начать игру!",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 

        20, 90, 240, 50,
        hwnd,
        (HMENU)LANDING_BUTTON_HMENU,
        hInstance,
        0
    );

    SendMessage(_editHWND, EM_SETLIMITTEXT, INPUT_NAME_LENGTH, 0);
}

LRESULT CALLBACK LoginWindow::LoginWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static LoginWindow* landing = nullptr;
    
    switch (uMsg)
    {
    case WM_CREATE:
        landing = (LoginWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);      //Id окна, вызвавшее событие
        int wmEvent = HIWORD(wParam);       //Id события
        if (wmId == LANDING_BUTTON_HMENU)
        {
            wchar_t name[INPUT_NAME_LENGTH];
            GetWindowText(landing->_editHWND, name, INPUT_NAME_LENGTH);
            landing->App->GetClient()->LoginUser(name);
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
        hFont1 = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
        
        SelectObject(hdc01, hFont);
        SetTextColor(hdc01, RGB(255, 255, 255));
        SetBkMode(hdc01, 1);
    	TextOut(hdc01, 35, 10, L"В дурачка?", wcslen(L"В дурачка?"));

        SelectObject(hdc01, hFont1);
        TextOut(hdc01, 20, 63, L"Ник:", wcslen(L"Ник:"));
        //EndPaint(hwnd, &ps01);

        EndPaint(hwnd, &ps01);
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void LoginWindow::RegisterWnd()
{
    WNDCLASS wc = { };

    wc.lpfnWndProc = LoginWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = LANDING_WINDOW_CLASS_NAME;

    RegisterClass(&wc);
}
