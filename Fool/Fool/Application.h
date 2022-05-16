#pragma once
//#include "GameClient.h"
#include "Global.h"
#include "GameWindow.h"
#include "LoginWindow.h"
#include "WaitingWindow.h"
//#include "Figure.h"

// ����� - ����������
class Application
{
private:
	// ���� ����������
	GameWindow* _game;
	LoginWindow* _login;
	WaitingWindow* _waiting;

	// ������ �� �������
	//GameClient* _client;

public:
	Application()
	{

		_login = new LoginWindow(this);
		_waiting = new WaitingWindow(this);
		_game = new GameWindow(this);

		//_client = new GameClient(this, (char*)SERVER_IP, SERVER_PORT);
	}

	//GameClient* GetClient() { return _client; }
	GameWindow* GetGameWindow() { return _game; }
	WaitingWindow* GetWaitingWindow() { return _waiting; }
	LoginWindow* GetLoginWindow() { return _login; }

	// ������ ��������� 1 �� 3� ��������� ���� ���������
	void Login();
	void Wait();
	void Game();


};

