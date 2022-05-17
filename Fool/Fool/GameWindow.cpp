#include "GameWindow.h"

#include <algorithm>
#include <iterator>

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

    this->_hBitoButton = CreateWindowEx(
        0,
        L"BUTTON",
        L"Бито!",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 

        TRUMP_MARGIN_LEFT, 500, CARD_WIDTH, 50,
        hwnd,
        (HMENU)BITO_HMENU,
        hInstance,
        0
    );

    this->_hBitoButton = CreateWindowEx(
        0,
        L"BUTTON",
        L"Взять",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 

        QUITED_MARGIN_LEFT, 500, CARD_WIDTH, 50,
        hwnd,
        (HMENU)TAKE_HMENU,
        hInstance,
        0
    );

    auto hMenu = 3000;
    for (int i = 0; i < CARDS_AMOUNT; i++) {
        _cards.push_back(new Card(app, &_cardPresets[i], hwnd, (HMENU)hMenu++));
    }

    // MoveCardsToDeck(int[] ids)   - придет с сервера
    std::copy(_cards.begin(), _cards.end(), std::back_inserter(_deck));
    DrawDeck();
    _trump = *(_cards.end()-1);
    DrawTrump();

    //test

    /*_cardsOnHand.push_back(*(_cards.begin() + 10));
    _cardsOnHand.push_back(*(_cards.begin() + 11));
    _cardsOnHand.push_back(*(_cards.begin() + 12));
    _cardsOnEnemyHand.push_back(*(_cards.begin() + 13));
    _cardsOnEnemyHand.push_back(*(_cards.begin() + 14));
    _cardsOnEnemyHand.push_back(*(_cards.begin() + 15));

    _quited.push_back(*(_cards.begin() + 20));
    _quited.push_back(*(_cards.begin() + 21));

    _cardsOnBoardWrap.push_back(new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, _cards[0]));
    _cardsOnBoardWrap.push_back(new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, _cards[1]));
    _cardsOnBoardWrap.back()->SetHittingCard(_cards[25]);
    _cardsOnBoardWrap.push_back(new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, _cards[2]));
    _cardsOnBoardWrap.back()->SetHittingCard(_cards[26]);
    _cardsOnBoardWrap.push_back(new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, _cards[3]));
    _cardsOnBoardWrap.back()->SetHittingCard(_cards[27]);
    _cardsOnBoardWrap.push_back(new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, _cards[4]));
    _cardsOnBoardWrap.push_back(new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, _cards[5]));

    _deck.push_back(_cards[34]);
    DrawHand();
    DrawEnemyHand();
    DrawQuited();
    DrawBoard();
    DrawDeck();*/
    //----------------------------
}

void GameWindow::DrawHand()
{
    int amountCardOnHand = _cardsOnHand.size();
    int cardDistance = GAME_WINDOW_WIDTH / amountCardOnHand;
    int leftMargin = (cardDistance - CARD_WIDTH) / 2;
    if (leftMargin < 0)
        leftMargin = 0;

    Card* card = nullptr;
	for (int i = 0; i < _cardsOnHand.size(); i++)
	{
        card = _cardsOnHand[i];

        card->Show();
        card->CardUp();
        card->MovePositionBottom(leftMargin + cardDistance * i, CARDS_HAND_MARGIN);
	}
}

void GameWindow::DrawEnemyHand()
{
    int amountCardOnHand = _cardsOnEnemyHand.size();
    int cardDistance = GAME_WINDOW_WIDTH / amountCardOnHand;
    int leftMargin = (cardDistance - CARD_WIDTH) / 2;
    if (leftMargin < 0)
        leftMargin = 0;

    Card* card = nullptr;
    for (int i = 0; i < _cardsOnEnemyHand.size(); i++)
    {
        card = _cardsOnEnemyHand[i];

        card->ShirtUp();
        card->Show();
        card->MovePositionBottom(leftMargin + cardDistance * i, CARDS_ENEMY_HAND_MARGIN);
    }
}

void GameWindow::DrawTrump()
{
    _trump->CardUp();
    _trump->Show();
    _trump->MovePositionTop(TRUMP_MARGIN_LEFT, TRUMP_MARGIN_TOP);
}

void GameWindow::DrawDeck()
{
    for(auto card : _deck)
    {
        card->ShirtUp();
        card->Show();
        card->MovePositionBottom(DECK_MARGIN_LEFT, DECK_MARGIN_TOP);
    }
}

void GameWindow::DrawQuited()
{
    for (auto card : _quited)
    {
        card->Show();
        card->ShirtUp();
        card->MovePositionTop(QUITED_MARGIN_LEFT, QUITED_MARGIN_TOP);
    }
}

void GameWindow::DrawBoard()
{
    for(int i = 0; i < _cardsOnBoardWrap.size(); i++)
    {
	    if(i < 3)
	    {
            int offsetX = CARD_BOARD_MARGIN_LEFT + i * (CARD_BOARD_DISTANSE + CARD_WIDTH);
            _cardsOnBoardWrap[i]->MovePosition(offsetX, CARD_BOARD_MARGIN_TOP_FIRST);
            _cardsOnBoardWrap[i]->Show();
	    }else
	    {
            int offsetX = CARD_BOARD_MARGIN_LEFT + (i-3) * (CARD_BOARD_DISTANSE + CARD_WIDTH);
            _cardsOnBoardWrap[i]->MovePosition(offsetX, CARD_BOARD_MARGIN_TOP_SECOND);
            _cardsOnBoardWrap[i]->Show();
	    }
    }
}

bool GameWindow::IsCardInHand(Card* card)
{
    if (std::find(_cardsOnHand.begin(), _cardsOnHand.end(), card) != _cardsOnHand.end())
        return true;
    else
        return false;
}

bool GameWindow::IsCardInEnemyHand(Card* card)
{
    if (std::find(_cardsOnEnemyHand.begin(), _cardsOnEnemyHand.end(), card) != _cardsOnEnemyHand.end())
        return true;
    else
        return false;
}

bool GameWindow::IsCardInDeck(Card* card)
{
    if (std::find(_deck.begin(), _deck.end(), card) != _deck.end())
        return true;
    else
        return false;
}

bool GameWindow::IsCardInQuited(Card* card)
{
    if (std::find(_quited.begin(), _quited.end(), card) != _quited.end())
        return true;
    else
        return false;
}

Card* GameWindow::GetCardById(int cardId)
{
    for (auto card : _cards)
        if (card->hMenu == (HMENU)cardId)
            return card;
    return nullptr;
}

void GameWindow::MoveCardFromDeckToHand(Card* card)
{
    auto finded = std::find(_deck.begin(), _deck.end(), card);
    if(finded != _deck.end())
    {
        _deck.erase(finded);
        _cardsOnHand.push_back(card);
    }
}

void GameWindow::MoveCardFromDeckToEnemyHand(Card* card)
{
    auto finded = std::find(_deck.begin(), _deck.end(), card);
    if (finded != _deck.end())
    {
        _deck.erase(finded);
        _cardsOnEnemyHand.push_back(card);
    }
}

void GameWindow::MoveCardFromHandToBoard(Card* card)
{
    auto finded = std::find(_cardsOnHand.begin(), _cardsOnHand.end(), card);
    if (finded != _cardsOnHand.end())
    {
        if (CheckPossibilityToAddCardToBoard(card)) {
            _cardsOnHand.erase(finded);
            _cardsOnBoard.push_back(card);

            auto addedCardWrap = new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, card);
            _cardsOnBoardWrap.push_back(addedCardWrap);
        }
    }
}

void GameWindow::MoveCardFromEnemyHandToBoard(Card* card)
{
    auto finded = std::find(_cardsOnEnemyHand.begin(), _cardsOnEnemyHand.end(), card);
    if (finded != _cardsOnEnemyHand.end())
    {
        if (CheckPossibilityToAddCardToBoard(card)) {
            _cardsOnEnemyHand.erase(finded);
            _cardsOnBoard.push_back(card);

            auto addedCardWrap = new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, card);
            _cardsOnBoardWrap.push_back(addedCardWrap);
        }
    }
}

void GameWindow::MoveCardFromHandToHit(Card* card, CardOnBoard* cardWrap){
    if(cardWrap->CheckPossibilityToHit(card))
    {
        auto finded = std::find(_cardsOnHand.begin(), _cardsOnHand.end(), card);
        if (finded != _cardsOnHand.end())
        {
            cardWrap->SetHittingCard(card);
            _cardsOnHand.erase(finded);
            _cardsOnBoard.push_back(card);
        }
    }
}

void GameWindow::MoveCardFromEnemyHandToHit(Card* card, CardOnBoard* cardWrap) {
    if (cardWrap->CheckPossibilityToHit(card))
    {
        auto finded = std::find(_cardsOnEnemyHand.begin(), _cardsOnEnemyHand.end(), card);
        if (finded != _cardsOnEnemyHand.end())
        {
            cardWrap->SetHittingCard(card);
            _cardsOnEnemyHand.erase(finded);
            _cardsOnBoard.push_back(card);
        }
    }
}

void GameWindow::MoveCardsFromBoadToHand()
{
    std::copy(_cardsOnBoard.begin(), _cardsOnBoard.end(), _cardsOnHand.end());
    _cardsOnBoardWrap.clear();
    _cardsOnBoard.clear();
}

void GameWindow::MoveCardsFromBoadToEnemyHand()
{
    std::copy(_cardsOnBoard.begin(), _cardsOnBoard.end(), _cardsOnEnemyHand.end());
    _cardsOnBoardWrap.clear();
    _cardsOnBoard.clear();
}

void GameWindow::MoveCardsFromBoadToQuited()
{
    std::copy(_cardsOnBoard.begin(), _cardsOnBoard.end(), _quited.end());
    _cardsOnBoardWrap.clear();
    _cardsOnBoard.clear();
}

void GameWindow::MoveCardsToDeck()
{
    _cardsOnHand.clear();
    _cardsOnEnemyHand.clear();
    _cardsOnBoard.clear();
    _quited.clear();
    _deck.clear();

    std::copy(_cards.begin(), _cards.end(), _deck.begin());
}

bool GameWindow::CardComparer(Card* card1, Card* card2)
{
    return card1->GetCardPreset()->CardNumber < card2->GetCardPreset()->CardNumber;
}

void GameWindow::SortCardsOnHand()
{
    std::sort(_cardsOnHand.begin(), _cardsOnHand.end(), &CardComparer);
}

void GameWindow::SortCardsOnEnemyHand()
{
    std::sort(_cardsOnEnemyHand.begin(), _cardsOnEnemyHand.end(), &CardComparer);
}

bool GameWindow::CheckPossibilityToAddCardToBoard(Card* card)
{
    bool retVal = false;
    if (_cardsOnBoardWrap.size() == 0)
        retVal = true;
    else
    {
        for(auto cardWrap : _cardsOnBoardWrap)
        {
	        if(cardWrap->GetCard()->GetCardPreset()->CardNumber == card->GetCardPreset()->CardNumber ||
                (cardWrap->GetHittingCard() != nullptr &&
                cardWrap->GetHittingCard()->GetCardPreset()->CardNumber == card->GetCardPreset()->CardNumber))
	        {
                retVal = true;
                break;
	        }
        }
    }
    return retVal;
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

        if (wmId >= 3000 && wmId < 3037)
        {
            Card* card = game->GetCardById(wmId);

            bool IsCardBeTrumpInDeckNotLast = (card == game->_trump) && (game->IsCardInDeck(card)) && (game->GetDeckSize() != 1);
            bool IsCardInHand = game->IsCardInHand(card);
            bool IsCardInEnemyHand = game->IsCardInEnemyHand(card);
            bool IsCardInDeck = game->IsCardInDeck(card);

            bool IsCardBeTrumpCard = card == game->_trump;

            bool IsDeckSizeEqualsOne = game->GetDeckSize() == 1;

            if((IsCardInDeck && !IsCardBeTrumpCard) || (IsCardInDeck && IsCardBeTrumpCard && IsDeckSizeEqualsOne))
            {
                game->MoveCardFromDeckToHand(card);
                game->DrawHand();
            } else if(IsCardInHand) {
                if(game->CheckPossibilityToAddCardToBoard(card))
                {
                    game->MoveCardFromHandToBoard(card);
                    game->DrawBoard();
                    game->DrawHand();
                }
            }

            if(IsCardInHand)

        }
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT     ps01;
        HDC             hdc01;
        BITMAP          bitmap01;
        HDC             hdcMem01;
        HGDIOBJ         oldBitmap01;
        HFONT           hFont;

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
        TextOut(hdc01, 75, 150, L"Бито", wcslen(L"Бито"));
        TextOut(hdc01, 820, 150, L"Колода", wcslen(L"Колода"));

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
