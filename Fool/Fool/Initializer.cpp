#include "Initializer.h"

#include "ConnectionWindow.h"
#include "GameWindow.h"
#include "LoginWindow.h"
#include "WaitingWindow.h"

void Init()
{
	InitWndClasses();
}

// Инициализирует классы окон
void InitWndClasses()
{
	LoginWindow::RegisterWnd();
	WaitingWindow::RegisterWnd();
	GameWindow::RegisterWnd();
	ConnectionWindow::RegisterWnd();
}