#pragma once
#include "Client.h"


enum COMMANDS { BROKE = 0, WAIT = 1, STARTGAME = 2, GAMERUNTIME = 3, ENDGAME = 4 };
enum WAITCOMMANDS { WAITFORSTART = 1 };
enum STARTGAMECOMMANDS { START = 1 };
enum GAMERUNTIMECOMMANDS { ENEMYCARDTOBOARD = 1, ENEMYHITCARD = 2, ENEMYPEAKCART = 3, ENEMYGIVECARDS = 4, BITO = 5, ENEMYDRAWCARDHITPREVIEWS = 6 };
enum ENDGAMECOMMANDS { WIN = 1, LOSE = 2, ENEMYLOSTCONNECTION = 3, DRAW = 4, INTERRUPT = 5 };

// Класс клиента нашей игры
class Application;
class FoolClient : Client
{
public:
	Application* App;

	FoolClient(Application* application, char* ip, int port) : Client(ip, port)
	{
		App = application;

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&FoolClient::MessageHandler, this, 0, 0);

	}

	static void MessageHandler(LPVOID LParam);

	int LoginUser(wchar_t* name);

	int EnemyCardToBoard(int cardId);
	int EnemyHitCard(int cardId, int hittedCardId);
	int EnemyPeakCard(int cardId);
	int EnemyGiveCards();
	int Bito();
	int EnemyDrawCardHitsPrevies(int cardId);

	int LoseGame();
	int DrawGame();
	int InterruptGame();
};

