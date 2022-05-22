#include "Application.h"

#include <string>

void Application::MakeFoolClient(char* ip, int port)
{
	try {
		_client = new FoolClient(this, ip, port);
	}
	catch (wchar_t mess[])
	{
		MessageBox(0, mess, MB_OK, 0);
	};
}

void Application::Connection()
{
	_game->Show(false);
	_waiting->Show(false);
	_login->Show(false);
	_connection->Show(true);
}

void Application::Login()
{
	_game->Show(false);
	_waiting->Show(false);
	_connection->Show(false);
	_login->Show();
}

void Application::Wait()
{
	_login->Show(false);
	_game->Show(false);
	_connection->Show(false);
	_waiting->Show();
}

void Application::Game()
{
	_login->Show(false);
	_waiting->Show(false);
	_connection->Show(false);
	_game->Show();
}
