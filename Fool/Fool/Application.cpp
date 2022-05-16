#include "Application.h"

void Application::Login()
{
	//_game->Show(false);
	_waiting->Show(false);
	_login->Show();
}

void Application::Wait()
{
	_login->Show(false);
	//_game->Show(false);
	_waiting->Show();
}

void Application::Game()
{
	_login->Show(false);
	_waiting->Show(false);
	_game->Show();
}
