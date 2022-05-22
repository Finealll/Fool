#pragma once
//#include "Figure.h"
//#include "Square.h"
#include <vector>

#include "Window.h"
#include "Global.h"
#include "framework.h"
#include "GameWindowDefines.h"

#include "Card.h"
#include "CardOnBoard.h"


#define GAME_WINDOW_CLASS_NAME L"GAME_WINDOW_CLASS_NAME"
#define GAME_WINDOW_NAME L"Кто же дурачек?"


const int INPUTNAMELENGTH = 16;

enum Player { ME = 1, ENEMY };

// Окно игры
class Application;
class GameWindow : public Window
{
private:
	CardPreset* _cardPresets;

	std::vector<Card*> _cards;
	std::vector<Card*> _deck;
	std::vector<Card*> _quited;
	std::vector<Card*> _cardsOnBoard;
	std::vector<Card*> _cardsOnHand;
	std::vector<Card*> _cardsOnEnemyHand;

	std::vector<CardOnBoard*> _cardsOnBoardWrap;

	HWND _cardHits[36];

	Card* _trump;

	HWND _hBitoButton;
	HWND _hTakeButton;
	
public:
	//FigureType playerFigure;
	Player currentPlayer;
	Player WhoI;
	wchar_t enemyName[INPUTNAMELENGTH];

	GameWindow(Application* app);

	// Рисование карт -------------------------------
public:
	void DrawHand();
	void DrawEnemyHand();

	void DrawTrump();
	void DrawDeck();
	void DrawQuited();

	void DrawBoard();
	void DrawHittedPreview(Card* card);
	void DrawHiddenHittedPreview(Card* card);
	void ClearHittedPreviews();
	//-----------------------------------------------

	// Проверка на нахождение карт в векторах -------
public:
	bool IsCardInHand(Card* card);
	bool IsCardInEnemyHand(Card* card);
	bool IsCardInDeck(Card* card);
	bool IsCardInQuited(Card* card);
	//-----------------------------------------------

	// Получение карты по hMenu
public:
	Card* GetCardById(int cardId);
	CardOnBoard* GetCardBoardByCard(Card* board);
	HWND GetCardHitHWNDByHMenu(int hMenu);

	// Получение размеров векторов -----------------
public:
	int GetDeckSize() { return _deck.size(); }
	//----------------------------------------------

	// Перемещения карт -----------------------------
public:
	void MoveCardFromDeckToHand(Card* card);
	void MoveCardFromDeckToEnemyHand(Card* card);

	void MoveCardFromHandToBoard(Card* card);
	void MoveCardFromEnemyHandToBoard(Card* card);

	void MoveCardFromHandToHit(Card* card, CardOnBoard* cardWrap);
	void MoveCardFromEnemyHandToHit(Card* card, CardOnBoard* cardWrap);

	void MoveCardsFromBoadToHand();
	void MoveCardsFromBoadToEnemyHand();
	void MoveCardsFromBoadToQuited();

	void MoveCardsToDeck();

	void MoveHandCardToWrapper(Card* card, CardOnBoard* cardOnBoard);
	void MoveEnemyHandCardToWrapper(Card* card, CardOnBoard* cardOnBoard);
	// ----------------------------------------------

	// Сортировка карт в векторах -------------------
private:
	static bool CardComparer(Card* card1, Card* card2);
public:
	void SortCardsOnHand();
	void SortCardsOnEnemyHand();
	//-----------------------------------------------

	// Работа с игровым столом
public:
	bool CheckPossibilityToAddCardToBoard(Card* card);
	bool CheckPossibilityToHitCard(Card* card);
	bool CheckBito();
	void PeekCardsToHand();
	void PeekCardsToEnemyHand();
	//-----------------------------------------------

	// Игровой процесс-------------------------------

	void StartGame(int cards[], wchar_t* enemyName, bool isFirst);
	bool CheckWin();
	void EndGame(wchar_t* message);
	void ToggleStep();
	//-----------------------------------------------
	//Square* GetSquareByHMenu(HMENU hMenu);

	//Figure* CreateFigureByXY(int x, int y, FigureType type);
	//void ClickSquare(int x, int y, FigureType figure);
	//void EnableSquares(bool enable);
	//void ToggleStep();


	//bool CheckWin(int len);
	//bool CheckDraw();

	static LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void RegisterWnd();
};

