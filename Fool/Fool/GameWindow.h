#pragma once
//#include "Figure.h"
//#include "Square.h"
#include "Window.h"
#include "Global.h"
#include "framework.h"
#include "GameWindowDefines.h"

#include "Card.h"


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
	//Square* _squares[FIGURES_ROWS][FIGURES_COLS];
	//Figure* _figures[FIGURES_ROWS][FIGURES_COLS];
public:
	//FigureType playerFigure;
	Player currentPlayer;
	wchar_t enemyName[INPUTNAMELENGTH];

	GameWindow(Application* app);

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

