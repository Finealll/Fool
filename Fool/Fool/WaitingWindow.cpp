#include "WaitingWindow.h"


WaitingWindow::WaitingWindow(Application* app) : Window(app)
{
    hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        WAITING_WINDOW_CLASS_NAME,                     // Window class
        WAITING_WINDOW_NAME,    // Window text
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 100,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        static_cast<LPVOID>(this)
    );

}

LRESULT CALLBACK WaitingWindow::WaitingWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static WaitingWindow* waiting = nullptr;

    switch (uMsg)
    {
    case WM_CREATE:
        waiting = (WaitingWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);      //Id окна, вызвавшее событие
        int wmEvent = HIWORD(wParam);       //Id событи€


    }
    case WM_PAINT:
    {
        PAINTSTRUCT     ps01;
        HDC             hdc01;
        BITMAP          bitmap01;
        HDC             hdcMem01;
        HGDIOBJ         oldBitmap01;
        HFONT hFont;

        HBITMAP hBitmap01 = (HBITMAP)LoadImage(NULL, L"background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


        hdc01 = BeginPaint(hwnd, &ps01);

        hdcMem01 = CreateCompatibleDC(hdc01);
        oldBitmap01 = SelectObject(hdcMem01, hBitmap01);

        GetObject(hBitmap01, sizeof(bitmap01), &bitmap01);
        BitBlt(hdc01, 0, 0, bitmap01.bmWidth, bitmap01.bmHeight, hdcMem01, 0, 0, SRCCOPY);

        SelectObject(hdcMem01, oldBitmap01);
        DeleteDC(hdcMem01);
        
        hFont = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");

        SelectObject(hdc01, hFont);
        SetTextColor(hdc01, RGB(255, 255, 255));
        SetBkMode(hdc01, 1);
        TextOut(hdc01, 55, 10, L"ќжидайте...", wcslen(L"ќжидайте..."));
        
        EndPaint(hwnd, &ps01);
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WaitingWindow::RegisterWnd()
{
    WNDCLASS wc = { };

    wc.lpfnWndProc = WaitingWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = WAITING_WINDOW_CLASS_NAME;

    RegisterClass(&wc);
}
