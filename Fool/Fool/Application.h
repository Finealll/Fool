#pragma once
//#include "GameClient.h"
#include "Global.h"
#include "GameWindow.h"
#include "LoginWindow.h"
#include "WaitingWindow.h"
#include "ConnectionWindow.h"
#include "FoolClient.h"
//#include "Figure.h"

// ����� - ����������
class Application
{
private:
	// ���� ����������
	GameWindow* _game;
	LoginWindow* _login;
	WaitingWindow* _waiting;
	ConnectionWindow* _connection;

	// ������ �� �������
	FoolClient* _client;

public:
	Application()
	{

		_login = new LoginWindow(this);
		_waiting = new WaitingWindow(this);
		_game = new GameWindow(this);
		_connection = new ConnectionWindow(this);
	}

	FoolClient* GetClient() { return _client; }
	GameWindow* GetGameWindow() { return _game; }
	WaitingWindow* GetWaitingWindow() { return _waiting; }
	LoginWindow* GetLoginWindow() { return _login; }
	ConnectionWindow* GetConnectionWindow() { return _connection; }
	
	void MakeFoolClient(char* ip, int port);

	// ������ ��������� 1 �� 3� ��������� ���� ���������
	void Connection();
	void Login();
	void Wait();
	void Game();


};

