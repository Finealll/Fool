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

    this->_hTakeButton = CreateWindowEx(
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
        _cards.push_back(new Card(app, &_cardPresets[i], hwnd, (HMENU)hMenu));
        _cardHits[i] = CreateWindowEx(
            0,
            L"BUTTON",
            L"+",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 

            0, 0, CARD_WIDTH, CARD_HEIGHT,
            hwnd,
            (HMENU)(hMenu + 400),
            GetModuleHandleA(NULL),
            0
        );
        ShowWindow(_cardHits[i], false);
        hMenu++;
    }
}

void GameWindow::DrawHand()
{
    int amountCardOnHand = _cardsOnHand.size();
    int cardDistance = amountCardOnHand != 0 ? GAME_WINDOW_WIDTH / amountCardOnHand : 0;
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
    int cardDistance = amountCardOnHand == 0 ? 0 : GAME_WINDOW_WIDTH / amountCardOnHand;
    int leftMargin = (cardDistance - CARD_WIDTH) / 2;
    if (leftMargin < 0)
        leftMargin = 0;

    Card* card = nullptr;
    for (int i = 0; i < _cardsOnEnemyHand.size(); i++)
    {
        card = _cardsOnEnemyHand[i];

        card->ShirtUp();
        card->Show();
        card->MovePositionTop(leftMargin + cardDistance * i, CARDS_ENEMY_HAND_MARGIN);
    }
}

void GameWindow::DrawTrump()
{
    if (IsCardInDeck(_trump)) {
        _trump->CardUp();
        _trump->Show();
        _trump->MovePositionTop(TRUMP_MARGIN_LEFT, TRUMP_MARGIN_TOP);
    }
}

void GameWindow::DrawDeck()
{
    for(int i = 0; i < _deck.size()-1; i++)
    {
        _deck[i]->ShirtUp();
        _deck[i]->Show();
        _deck[i]->MovePositionBottom(DECK_MARGIN_LEFT, DECK_MARGIN_TOP);
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
            _cardsOnBoardWrap[i]->GetCard()->CardUp();
            if(_cardsOnBoardWrap[i]->GetHittingCard() != nullptr)
                _cardsOnBoardWrap[i]->GetHittingCard()->CardUp();
            _cardsOnBoardWrap[i]->Show();
	    }else
	    {
            int offsetX = CARD_BOARD_MARGIN_LEFT + (i-3) * (CARD_BOARD_DISTANSE + CARD_WIDTH);
            _cardsOnBoardWrap[i]->MovePosition(offsetX, CARD_BOARD_MARGIN_TOP_SECOND);
            _cardsOnBoardWrap[i]->GetCard()->CardUp();
            if (_cardsOnBoardWrap[i]->GetHittingCard() != nullptr)
                _cardsOnBoardWrap[i]->GetHittingCard()->CardUp();
            _cardsOnBoardWrap[i]->Show();
	    }
        _cardsOnBoardWrap[i]->DeleteHitPrevies();
    }
}

void GameWindow::DrawHittedPreview(Card* card)
{
    ClearHittedPreviews();
    for(auto cardWrap : _cardsOnBoardWrap)
    {
        if(cardWrap->CheckPossibilityToHit(card))
        {
            cardWrap->CreateHitPrevies(card, true);
        }
    }
}

void GameWindow::DrawHiddenHittedPreview(Card* card)
{
    ClearHittedPreviews();
    for (auto cardWrap : _cardsOnBoardWrap)
    {
        if (cardWrap->CheckPossibilityToHit(card))
        {
            cardWrap->CreateHitPrevies(card, false);
        }
    }
}

void GameWindow::ClearHittedPreviews()
{
	for(auto cardWraps : _cardsOnBoardWrap)
	{
        cardWraps->DeleteHitPrevies();
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

CardOnBoard* GameWindow::GetCardBoardByCard(Card* card)
{
    for (auto cardWrap : _cardsOnBoardWrap)
        if (cardWrap->GetCard() == card)
            return cardWrap;
    return nullptr;
}

HWND GameWindow::GetCardHitHWNDByHMenu(int hMenu)
{
    return _cardHits[hMenu - 3000];
}

void GameWindow::MoveCardFromDeckToHand(Card* card)
{
    auto finded = std::find(_deck.begin(), _deck.end(), card);
    if(finded != _deck.end())
    {
        (*finded)->Show(false);
        _deck.erase(finded);
        _cardsOnHand.push_back(card);
    }
}

void GameWindow::MoveCardFromDeckToEnemyHand(Card* card)
{
    auto finded = std::find(_deck.begin(), _deck.end(), card);
    if (finded != _deck.end())
    {
        (*finded)->Show(false);
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
            (*finded)->Show(false);
            _cardsOnHand.erase(finded);
            _cardsOnBoard.push_back(card);

            auto addedCardWrap = new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, card, GetCardHitHWNDByHMenu((int)card->hMenu));
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
            card->Show(false);
            _cardsOnEnemyHand.erase(finded);
            _cardsOnBoard.push_back(card);

            auto addedCardWrap = new CardOnBoard(this, _trump->GetCardPreset()->CardSuit, card, GetCardHitHWNDByHMenu((int)card->hMenu));
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
    std::copy(_cardsOnBoard.begin(), _cardsOnBoard.end(), std::back_inserter(_cardsOnHand));
    _cardsOnBoardWrap.clear();
    _cardsOnBoard.clear();
}

void GameWindow::MoveCardsFromBoadToEnemyHand()
{
    std::copy(_cardsOnBoard.begin(), _cardsOnBoard.end(), std::back_inserter(_cardsOnEnemyHand));
    _cardsOnBoardWrap.clear();
    _cardsOnBoard.clear();
}

void GameWindow::MoveCardsFromBoadToQuited()
{
    std::copy(_cardsOnBoard.begin(), _cardsOnBoard.end(), std::back_inserter(_quited));
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
    else if (_cardsOnBoardWrap.size() == 6)
        retVal = false;
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

void GameWindow::MoveHandCardToWrapper(Card* card, CardOnBoard* cardOnBoard)
{
    auto finded = std::find(_cardsOnHand.begin(), _cardsOnHand.end(), card);
    if (finded != _cardsOnHand.end())
    {
        cardOnBoard->SetHittingCard(card);
        _cardsOnHand.erase(finded);
        _cardsOnBoard.push_back(card);
    }
}

void GameWindow::MoveEnemyHandCardToWrapper(Card* card, CardOnBoard* cardOnBoard)
{
    auto finded = std::find(_cardsOnEnemyHand.begin(), _cardsOnEnemyHand.end(), card);
    if (finded != _cardsOnEnemyHand.end())
    {
        cardOnBoard->SetHittingCard(card);
        _cardsOnEnemyHand.erase(finded);
        _cardsOnBoard.push_back(card);
    }
}


bool GameWindow::CheckPossibilityToHitCard(Card* card)
{
    bool retVal = false;
    if (_cardsOnBoardWrap.size() == 0)
        retVal = false;
    else
    {
        for (auto cardWrap : _cardsOnBoardWrap)
        {
            if(cardWrap->GetHittingCard() == nullptr && cardWrap->CheckPossibilityToHit(card))
            {
                retVal = true;
                break;
            }
        }
    }
    return retVal;
}

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

        bool stepPosibility = (game->currentPlayer == game->WhoI);

        if (wmId >= 3000 && wmId < 3037)
        {
            Card* card = game->GetCardById(wmId);

            bool IsCardBeTrumpInDeckNotLast = (card == game->_trump) && (game->IsCardInDeck(card)) && (game->GetDeckSize() != 1);
            bool IsCardInHand = game->IsCardInHand(card);
            bool IsCardInEnemyHand = game->IsCardInEnemyHand(card);
            bool IsCardInDeck = game->IsCardInDeck(card);

            bool IsCardBeTrumpCard = card == game->_trump;

            bool IsDeckSizeEqualsOne = game->GetDeckSize() == 1;

            if (stepPosibility && IsCardInHand) {
                if (game->CheckPossibilityToAddCardToBoard(card))
                {
                    game->MoveCardFromHandToBoard(card);
                    game->DrawBoard();
                    game->DrawHand();
                    game->App->GetClient()->EnemyCardToBoard(wmId);

                    if(game->CheckWin())
                    {
                        game->EndGame((wchar_t*)L"Вы выиграли!");
                        game->App->GetClient()->LoseGame();
                    }
                }
            }
            else if (!stepPosibility){
                game->ClearHittedPreviews();
                if (game->CheckPossibilityToHitCard(card)) {
                    game->DrawBoard();
                    game->DrawHittedPreview(card);
                }
                game->App->GetClient()->EnemyDrawCardHitsPrevies(wmId);
            }
        	else if(IsCardInEnemyHand){}

           
	            

        } else if ((wmId >= 3000 + 400) && (wmId < 3037 + 400))
        {
            Card* card = game->GetCardById(wmId - 400);
            CardOnBoard* cardOnBoard = game->GetCardBoardByCard(card);

            Card* hittingCard = cardOnBoard->GetHittingCardPreview();

            game->MoveHandCardToWrapper(hittingCard, cardOnBoard);
            game->DrawBoard();
            game->DrawHand();
            game->App->GetClient()->EnemyHitCard((int)card->hMenu, (int)hittingCard->hMenu);

            if (game->CheckWin())
            {
                game->EndGame((wchar_t*)L"Вы выиграли!");
                game->App->GetClient()->LoseGame();
            }
        } else if(wmId == BITO_HMENU && stepPosibility)
        {
            if ( game->CheckBito()) {
                game->MoveCardsFromBoadToQuited();
                game->DrawQuited();
                game->DrawBoard();
                game->App->GetClient()->Bito();

                game->PeekCardsToEnemyHand();
                game->PeekCardsToHand();
                game->DrawHand();
                game->DrawEnemyHand();

                game->ToggleStep();
            }
        }
        else if (wmId == TAKE_HMENU && !stepPosibility)
        {
            game->MoveCardsFromBoadToHand();
            game->DrawHand();
            game->DrawBoard();
            game->App->GetClient()->EnemyGiveCards();


            game->PeekCardsToEnemyHand();
            game->DrawHand();
            game->DrawEnemyHand();
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

        return 0;
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

bool GameWindow::CheckBito()
{
    bool retVal = true;
    if (_cardsOnBoard.size() == 0)
        retVal = false;
    else {
        for (auto cardWrap : _cardsOnBoardWrap)
        {
            if (!cardWrap->IsHitted())
            {
                retVal = false;
                break;
            }
        }
    }
    return retVal;
}

void GameWindow::PeekCardsToHand()
{
    for(int i = _cardsOnHand.size(); i < 6; i++)
    {
        if (!_deck.empty())
			MoveCardFromDeckToHand(_deck.front());
    }
}

void GameWindow::PeekCardsToEnemyHand()
{
    for (int i = _cardsOnEnemyHand.size(); i < 6; i++)
    {
        if(!_deck.empty())
			MoveCardFromDeckToEnemyHand(_deck.front());
    }
}

void GameWindow::EndGame(wchar_t* message)
{
    MessageBox(hwnd, message, L"Сообщение", 0);
    App->Login();
}

void GameWindow::ToggleStep()
{
    if(currentPlayer == ME)
    {
        currentPlayer = ENEMY;
    } else
    {
        currentPlayer = ME;
    }

    wchar_t title[100]{};
    wcscat_s(title, L"Противник: ");
    wcscat_s(title, enemyName);
    if ((currentPlayer == WhoI))
        wcscat_s(title, L", ваш ход");
    else
        wcscat_s(title, L", ход противника");
    SetWindowText(hwnd, title);
}

void GameWindow::StartGame(int cards[], wchar_t* enemyName, bool isFirst)
{
	wcscpy_s(this->enemyName, enemyName);

    wchar_t title[100]{};
    wcscat_s(title, L"Противник: ");
    wcscat_s(title, enemyName);
    if(isFirst)
        wcscat_s(title, L", ваш ход");
    else
        wcscat_s(title, L", ход противника");
    SetWindowText(hwnd, title);

    _deck.clear();
    _cardsOnBoardWrap.clear();
    _cardsOnHand.clear();
    _cardsOnEnemyHand.clear();
    _cardsOnBoard.clear();
    _quited.clear();

    currentPlayer = ME;
    WhoI = isFirst ? ME : ENEMY;

    for(int i = 0; i < 36; i++)
    {
        Card* card = nullptr;
        for(Card* cardIter : this->_cards)
        {
	        if(cardIter -> GetCardPreset()->SystemNumber == cards[i])
	        {
                _deck.push_back(cardIter);
                cardIter->ShirtUp();
                break;
	        }
        }
    }
    if (isFirst)
    {
        for(int i = 0; i < 6; i++)
        {
            MoveCardFromDeckToHand(_deck.front());
        }
        for (int i = 0; i < 6; i++)
        {
            MoveCardFromDeckToEnemyHand(_deck.front());
        }
    } else
    {
        for (int i = 0; i < 6; i++)
        {
            MoveCardFromDeckToEnemyHand(_deck.front());
        }
        for (int i = 0; i < 6; i++)
        {
            MoveCardFromDeckToHand(_deck.front());
        }
    }

    _trump = _deck.back();

    DrawTrump();
    DrawDeck();
    DrawHand();
    DrawEnemyHand();
    DrawQuited();
}

bool GameWindow::CheckWin()
{
    bool retVal = false;
    bool stepPosibility = (currentPlayer == WhoI);
    if (_deck.size() == 0 && _cardsOnHand.size() == 0)
        retVal = true;
    else
        retVal = false;
    return retVal;
}
