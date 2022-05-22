
#include "framework.h"
#include "Fool.h"

#include "Application.h"
#include "Initializer.h"

// Точка входа в приложение
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Init();        // Инитим классы окон

    const auto app = new Application();        // Запускаем приложение
    app->GetConnectionWindow()->Show();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

