#pragma once
//#include "Figure.h"
//#include "Square.h"
#include <vector>

#include "Window.h"
#include "Global.h"
#include "framework.h"
#include "GameWindowDefines.h"

#include "Card.h"


#define GAME_WINDOW_CLASS_NAME L"GAME_WINDOW_CLASS_NAME"
#define GAME_WINDOW_NAME L"��� �� �������?"


const int INPUTNAMELENGTH = 16;

enum Player { ME = 1, ENEMY };

// ���� ����
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
	Card* _trump;
	//Square* _squares[FIGURES_ROWS][FIGURES_COLS];
	//Figure* _figures[FIGURES_ROWS][FIGURES_COLS];
	
public:
	//FigureType playerFigure;
	Player currentPlayer;
	wchar_t enemyName[INPUTNAMELENGTH];

	GameWindow(Application* app);

	// ��������� ���� -------------------------------
	void DrawHand();
	void DrawEnemyHand();
	void DrawHandWithSelectedCard(int cardId);

	void DrawTrump();
	void DrawDeck();
	//-----------------------------------------------

	// �������� �� ���������� ���� � �������� -------
	bool IsCardInHand(Card* card);
	bool IsCardInEnemyHand(Card* card);
	bool IsCardInDeckHand(Card* card);
	//-----------------------------------------------

	// ��������� ����� �� hMenu
	Card* GetCardById(int cardId);

	// ����������� ���� -----------------------------
	void MoveCardFromDeckToHand(Card* card);
	void MoveCardFromDeckToEnemyHand(Card* card);

	void MoveCardFromHandToBoard(Card* card);
	void MoveCardFromEnemyHandToBoard(Card* card);

	void MoveCardsFromBoadToHand();
	void MoveCardsFromBoadToEnemyHand();
	void MoveCardsFromBoadToQuited();

	void MoveCardsToDeck();
	// ----------------------------------------------

	// ���������� ���� � �������� -------------------
private:
	bool CardComparer(Card* card1, Card* card2);
public:
	void SortCardsOnHand();
	void SortCardsOnEnemyHand();
	//-----------------------------------------------



	//Square* GetSquareByHMenu(HMENU hMenu);

	//Figure* CreateFigureByXY(int x, int y, FigureType type);
	//void ClickSquare(int x, int y, FigureType figure);
	//void EnableSquares(bool enable);
	//void ToggleStep();

	//void StartGame(FigureType type, wchar_t* enemyName);
	//void EndGame(wchar_t* message);

	//bool CheckWin(int len);
	//bool CheckDraw();

	static LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void RegisterWnd();
};

