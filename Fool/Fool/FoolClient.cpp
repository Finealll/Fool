#include "FoolClient.h"

#include <iostream>
#include <queue>

#include "Application.h"

int FoolClient::LoginUser(wchar_t* name)
{
    //���������� ��������� ��� ������ �����
    // [0] = 1, [1] = 0, [2] = 1 [32-64) - ���

    char message[MESSAGE_SIZE]{};
    message[0] = 1;
    message[1] = 1;
    message[2] = 0;

    memcpy(&message[32], name, wcslen(name) * sizeof(wchar_t));

    SendData(message);
    return 0;
}

int FoolClient::EnemyCardToBoard(int cardId)
{
    //���������� ��������� � ��� ��� ������������ ������� ����� �� ����
    // [0] = 3, [1] = 1, [3-7) - Id ���������� �����

    char message[MESSAGE_SIZE]{};
    message[0] = 3;
    message[1] = 1;
    message[2] = 0;

    memcpy(&message[3], &cardId, sizeof(int));

    SendData(message);
    return 0;
}

int FoolClient::EnemyHitCard(int cardId, int hittedCardId)
{
    //���������� ��������� � ��� ��� ������������ ����� �����, ���������� �� ����
    // [0] = 3, [1] = 2, [3-7) - Id ������������ �����, [8-12) - id ���������� �����

    char message[MESSAGE_SIZE]{};
    message[0] = 3;
    message[1] = 2;
    message[2] = 0;

    memcpy(&message[3], &cardId, sizeof(int));
    memcpy(&message[8], &hittedCardId, sizeof(int));

    SendData(message);
    return 0;
}

int FoolClient::EnemyPeakCard(int cardId)
{
    //���������� ��������� � ��� ��� ������������ ���� �����
    // [0] = 3, [1] = 3, [3-7) - Id ������ �����

    char message[MESSAGE_SIZE]{};
    message[0] = 3;
    message[1] = 3;
    message[2] = 0;

    memcpy(&message[3], &cardId, sizeof(int));

    SendData(message);
    return 0;
}

int FoolClient::EnemyGiveCards()
{
    //���������� ��������� � ��� ��� ������������ ���� ����� ����
    // [0] = 3, [1] = 4

    char message[MESSAGE_SIZE]{};
    message[0] = 3;
    message[1] = 4;
    message[2] = 0;
    

    SendData(message);
    return 0;
}

int FoolClient::Bito()
{
    //���������� ��������� � ��� ��� ������������ ���� ����� ����
    // [0] = 3, [1] = 5

    char message[MESSAGE_SIZE]{};
    message[0] = 3;
    message[1] = 5;
    message[2] = 0;


    SendData(message);
    return 0;
}

int FoolClient::EnemyDrawCardHitsPrevies(int cardId)
{
    //���������� ��������� � ��� ��� ������������ ���������� ���������� hit previews
    // [0] = 3, [1] = 6 [3-7) - cardId

    char message[MESSAGE_SIZE]{};
    message[0] = 3;
    message[1] = 6;
    message[2] = 0;

    memcpy(&message[3], &cardId, sizeof(int));


    SendData(message);
    return 0;
}

int FoolClient::LoseGame()
{
    //���������� ��������� � ���������� � ���������
    // [0] = 4, [1] = 2 

    char message[MESSAGE_SIZE]{};
    message[0] = 4;
    message[1] = 2;
    message[2] = 0;


    SendData(message);
    return 0;
}


void FoolClient::MessageHandler(LPVOID LParam)
{
    FoolClient* client = (FoolClient*)LParam;
    char data[MESSAGE_SIZE];
    int command, command1, command2;

    while (true) {
        memset(data, '\0', MESSAGE_SIZE);
        if (recv(client->GetSocket(), data, sizeof(data), 0))
        {
            command = data[0]; command1 = data[1]; command2 = data[2];
            std::cout << command << " " << command1 << " " << command2 << std::endl;

            if (command == BROKE)
                break;
            else {
                switch (command)
                {
                case WAIT:
                    switch (command1)
                    {
                    case WAITFORSTART:
                        client->App->Wait();
                        break;
                    }
                    break;
                case STARTGAME:
                    switch (command1)
                    {
                    case START: {
                        wchar_t Name[16]{};
                        memcpy(Name, &data[64], 32);
                        int shuffledCards[36];
                        int offset = 4;
                        for(int i = 0; i < 36; i++)
                        {
                            shuffledCards[i] = data[i + offset];
                        }
                        bool IsFirst = data[3] == 1;

                        client->App->GetGameWindow()->StartGame(shuffledCards, Name, IsFirst);
                        client->App->Game();

                        break;
                    }
                    }
                    break;
                case GAMERUNTIME:
                    
                    switch (command1)
                    {
	                case ENEMYCARDTOBOARD:
		                {
                        int cardId;
                        memcpy(&cardId, &data[3], sizeof(int));
                        Card* card = client->App->GetGameWindow()->GetCardById(cardId);
                        client->App->GetGameWindow()->MoveCardFromEnemyHandToBoard(card);
                        client->App->GetGameWindow()->DrawBoard();
                        client->App->GetGameWindow()->DrawEnemyHand();
                        SendMessage(client->App->GetGameWindow()->GetHwnd(), WM_SETREDRAW, TRUE, 0);
                        
                        break;
		                }
                    case ENEMYHITCARD:
                    {
                        int cardId, hittedCardId;
                        memcpy(&cardId, &data[3], sizeof(int));
                        memcpy(&hittedCardId, &data[8], sizeof(int));

                        Card* card = client->App->GetGameWindow()->GetCardById(cardId);
                        CardOnBoard* cardOnBoard = client->App->GetGameWindow()->GetCardBoardByCard(card);

                        Card* hittingCard = cardOnBoard->GetHittingCardPreview();

                        client->App->GetGameWindow()->MoveEnemyHandCardToWrapper(hittingCard, cardOnBoard);
                        client->App->GetGameWindow()->DrawBoard();
                        client->App->GetGameWindow()->DrawEnemyHand();
                        break;
                    }
	                case ENEMYDRAWCARDHITPREVIEWS:
			        {
                        int cardId;
                        memcpy(&cardId, &data[3], sizeof(int));
                        Card* card = client->App->GetGameWindow()->GetCardById(cardId);

                        client->App->GetGameWindow()->ClearHittedPreviews();
                        if (client->App->GetGameWindow()->CheckPossibilityToHitCard(card)) {
                            client->App->GetGameWindow()->DrawBoard();
                            client->App->GetGameWindow()->DrawHiddenHittedPreview(card);
                        }
	                    break;
			        }
	                case ENEMYPEAKCART:
	                {
	                    break;
	                }
	                case ENEMYGIVECARDS:
	                {

                        client->App->GetGameWindow()->MoveCardsFromBoadToEnemyHand();
                        client->App->GetGameWindow()->DrawEnemyHand();
                        client->App->GetGameWindow()->DrawBoard();

                        client->App->GetGameWindow()->PeekCardsToHand();
                        client->App->GetGameWindow()->DrawHand();
                        client->App->GetGameWindow()->DrawEnemyHand();
	                    break;
	                }
	                case BITO:
	                {
                        client->App->GetGameWindow()->MoveCardsFromBoadToQuited();
                        client->App->GetGameWindow()->DrawQuited();
                        client->App->GetGameWindow()->DrawBoard();

                        client->App->GetGameWindow()->PeekCardsToHand();
                        client->App->GetGameWindow()->PeekCardsToEnemyHand();
                        client->App->GetGameWindow()->DrawHand();
                        client->App->GetGameWindow()->DrawEnemyHand();

                        client->App->GetGameWindow()->ToggleStep();
	                    break;
	                }
                }
                break;
                case ENDGAME:
                    switch (command1)
                    {
                    case WIN:
                        //client->App->GetGameWindow()->EndGame((wchar_t*)L"�� ��������!");
                        break;
                    case LOSE:
                        client->App->GetGameWindow()->EndGame((wchar_t*)L"�� ���������!");
                        break;
                    case ENEMYLOSTCONNECTION:
                        client->App->GetGameWindow()->EndGame((wchar_t*)L"��������� ������� ����������, �� ��������!");
                        break;
                    case DRAW:
                        //client->App->GetGameWindow()->EndGame((wchar_t*)L"�����!");
                        break;
                    case INTERRUPT:
                        //client->App->GetGameWindow()->EndGame((wchar_t*)L"��������� ����� �� ����, �� ��������!");
                        break;
                    }
                }
            }
        };
    }
}
