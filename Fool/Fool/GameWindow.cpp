#include "GameWindow.h"

#include "Application.h"

GameWindow::GameWindow(Application* app) : Window(app)
{
    this->_cardPresets = GetCardPresets();

    this->hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        GAME_WINDOW_CLASS_NAME,                     // Window class
        GAME_WINDOW_NAME,    // Window text
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT,
        GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        static_cast<LPVOID>(this)
    );
    for (int i = 0; i < 37; i++) {
        auto card = new Card(app, &_cardPresets[i], hwnd, (HMENU)3000);
        card->Show();
        card->MovePosition(40 * i, 0);
    }
}

//Square* GameWindow::GetSquareByHMenu(HMENU hMenu)
//{
//    for (int i = 0; i < FIGURES_ROWS; i++)
//        for (int j = 0; j < FIGURES_COLS; j++)
//            if (_squares[i][j] != nullptr && _squares[i][j]->hMenu == hMenu)
//                return _squares[i][j];
//    return nullptr;
//}
//
//Figure* GameWindow::CreateFigureByXY(int x, int y, FigureType type)
//{
//    if (type == X) {
//        _figures[x][y]->MakeX();
//    }
//    else {
//        _figures[x][y]->MakeO();
//    }
//
//    return _figures[x][y];
//}
//
//void GameWindow::ClickSquare(int x, int y, FigureType type)
//{
//    CreateFigureByXY(x, y, type)->Show();
//    _squares[x][y]->Show(false);
//    ToggleStep();
//}
//
//void GameWindow::EnableSquares(bool enable)
//{
//    for (int i = 0; i < FIGURES_ROWS; i++)
//        for (int j = 0; j < FIGURES_COLS; j++)
//            _squares[i][j]->Enable(enable);
//}
//
//void GameWindow::ToggleStep()
//{
//    wchar_t buff[100] = L"Противник - ";
//    wcscat(buff, enemyName);
//
//    if (currentPlayer == ME) {
//        wcscat(buff, L". Ваш ход.");
//        EnableSquares(true);
//        currentPlayer = ENEMY;
//    }
//    else {
//        wcscat(buff, L". Ход противника.");
//        EnableSquares(false);
//        currentPlayer = ME;
//    }
//
//    SetWindowText(hwnd, buff);
//}
//
//void GameWindow::StartGame(FigureType type, wchar_t* enemyName)
//{
//    for (int i = 0; i < FIGURES_ROWS; i++)
//        for (int j = 0; j < FIGURES_COLS; j++)
//        {
//            _figures[i][j]->type = UNKNOWN;
//            _figures[i][j]->Show(false);
//            _squares[i][j]->Show(true);
//        }
//
//    wcscpy(this->enemyName, enemyName);
//    playerFigure = type;
//    currentPlayer = type == X ? ME : ENEMY;
//
//    ToggleStep();
//}
//
//void GameWindow::EndGame(wchar_t* message)
//{
//    MessageBox(0, message, MB_OK, 0);
//    App->Start();
//}

//bool GameWindow::CheckWin(int len = 5)
//{
//    for (int i = 0; i < FIGURES_ROWS; i++)
//    {
//        for (int j = 0; j < FIGURES_COLS; j++)
//        {
//            // Проверка на len вправо
//            if (len + j <= FIGURES_COLS)
//            {
//                bool isWin = true;
//                for (int k = j; k < j + len; k++)
//                    if (_figures[i][k]->type != playerFigure)
//                    {
//                        isWin = false;
//                        break;
//                    }
//                if (isWin) return true;
//            }
//
//            // Проверка на len вниз
//            if (len + i <= FIGURES_ROWS)
//            {
//                bool isWin = true;
//                for (int k = i; k < i + len; k++)
//                    if (_figures[k][j]->type != playerFigure)
//                    {
//                        isWin = false;
//                        break;
//                    }
//                if (isWin) return true;
//            }
//
//            // Проверка на len по диагонали вправо вниз
//            if (len + i <= FIGURES_ROWS && len + j <= FIGURES_COLS)
//            {
//                bool isWin = true;
//                for (int k = i; k < i + len; k++)
//                    if (_figures[k][k]->type != playerFigure)
//                    {
//                        isWin = false;
//                        break;
//                    }
//                if (isWin) return true;
//            }
//
//            // Проверка на len по диагонали влево вниз
//            if (len + i <= FIGURES_ROWS && j - len >= -1)
//            {
//                bool isWin = true;
//                for (int k = j, x = i; k > j - len && x < i + len; k--, x++)
//                    if (_figures[x][k]->type != playerFigure)
//                    {
//                        isWin = false;
//                        break;
//                    }
//                if (isWin) return true;
//            }
//
//        }
//    }
//
//    return false;
//}
//
//bool GameWindow::CheckDraw()
//{
//    for (int i = 0; i < FIGURES_ROWS; i++)
//        for (int j = 0; j < FIGURES_COLS; j++)
//            if (_figures[i][j]->type != UNKNOWN)
//                return false;
//    return true;
//}

LRESULT CALLBACK GameWindow::GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static GameWindow* game = nullptr;

    switch (uMsg)
    {
    case WM_CREATE:
        game = (GameWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);      //Id окна, вызвавшее событие
        int wmEvent = HIWORD(wParam);       //Id события

        /*if (wmId >= 3100 && wmId < 3200)
        {
            auto square = game->GetSquareByHMenu((HMENU)wmId);
            game->ClickSquare(square->posX, square->posY, game->playerFigure);
            game->App->GetClient()->SquareClick(square->posX, square->posY, game->playerFigure);
            if (game->CheckWin())
            {
                game->App->GetClient()->EndGame();
                game->EndGame((wchar_t*)L"Вы выиграли!");
            }
            if (game->CheckDraw())
            {
                game->App->GetClient()->DrawGame();
                game->EndGame((wchar_t*)L"Ничья!");
            }

        }*/
        return 0;
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
        
        EndPaint(hwnd, &ps01);
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void GameWindow::RegisterWnd()
{
    WNDCLASS wc = { };

    wc.lpfnWndProc = GameWindowProc;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.lpszClassName = GAME_WINDOW_CLASS_NAME;

    RegisterClass(&wc);
}
