#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <cstdio> 
#include <cstring> 
#include <vector>
#include <winsock2.h>
#include <mutex>
#include "Commands.h"
#include "Resolvers.h"

#pragma comment(lib, "WS2_32.lib")


using namespace std;


#define SERVER_PORT 9021

mutex g_lock;		//Локер для нашего вектора


DWORD WINAPI serverSend(SOCKET socket, char* mess) { //Отправка данных клиенту
	if (send(socket, mess, MESSAGE_SIZE, 0) == SOCKET_ERROR) {
		cout << "send failed with error " << WSAGetLastError() << endl;
		return -1;
	}
	return 1;
}


int CloseSocket(SOCKET socket)
{
	if (closesocket(socket) == SOCKET_ERROR) { //Ошибка закрытия сокета
		cout << "Close socket failed with error: " << WSAGetLastError() << endl;
		return -1;
	}
	WSACleanup();
	return 0;
}

struct User
{
	wchar_t Name[16]{};
	SOCKET Socket;
	User* PairUser;
	bool isSearch = false;
	bool isInGame = false;
};

vector<User*> waitingUsers;

DWORD WINAPI serverReceive(LPVOID lpParam) { //Получение данных от клиента
	char buffer[MESSAGE_SIZE]{ }; //Буфер для данных
	char response[MESSAGE_SIZE]{ }; //Буфер для ответа
	SOCKET client = *(SOCKET*)lpParam; //Сокет для клиента
	User user;
	user.Socket = client;
	int command, subcommand1, subcommand2;

	
	while (true) { //Цикл работы сервера
		if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			//Если не удалось получить данные буфера, сообщить об ошибке и выйти
			cout << "recv function failed with error " << WSAGetLastError() << endl;

			// Разместить проверки на поиск и игру
			if (user.isSearch)
			{
				g_lock.lock();
				waitingUsers.erase(std::remove(waitingUsers.begin(), waitingUsers.end(), &user), waitingUsers.end());
				g_lock.unlock();
				cout << user.Name << " вышел из очереди на игру";
			}

			if (user.isInGame)
			{
				response[0] = ENDGAME;
				response[1] = ENEMYLOSTCONNECTION;
				if (user.PairUser->Socket)
					serverSend(user.PairUser->Socket, response);
				user.PairUser->isInGame = false;
			}

			CloseSocket(client);

			return 1;
		}
		command = buffer[0];
		subcommand1 = buffer[1];
		subcommand2 = buffer[2];
		cout << user.Socket << " " << command << " " << subcommand1 << " " << subcommand2 << endl;


		switch (command)
		{
		case WAIT:
			if (subcommand1 == WAITFORSTART)
			{
				memcpy(&user.Name, &buffer[32], 32);
				wcout << user.Name;
				cout << " встает в очередь на игру\n";

				g_lock.lock();
				waitingUsers.push_back(&user);
				user.isSearch = true;
				if (waitingUsers.size() < 2)
				{
					response[0] = WAIT;
					response[1] = WAITFORSTART;
					serverSend(client, response);
				}
				else
				{
					User* user1 = waitingUsers.back();
					waitingUsers.pop_back();
					User* user2 = waitingUsers.back();
					waitingUsers.pop_back();
					user1->PairUser = user2;
					user1->isSearch = false;
					user1->isInGame = true;
					user2->PairUser = user1;
					user2->isSearch = false;
					user2->isInGame = true;

					wcout << user1->Name;
					cout << " и ";
					wcout << user2->Name;
					cout << " вышли из очереди и стартуют игру\n";

					// Отправка данных о старте игры
					memcpy(response, GetCommandForStart(), MESSAGE_SIZE); 
					response[0] = STARTGAME;
					response[1] = START;
					response[3] = 1;
					memcpy(&response[64], user2->Name, 32);
					serverSend(user1->Socket, response);

					response[3] = 2;
					memcpy(&response[64], user1->Name, 32);
					serverSend(user2->Socket, response);
				}

				g_lock.unlock();
				break;
			}
			break;
		case GAMERUNTIME:
			serverSend(user.PairUser->Socket, buffer);
			break;
		case ENDGAME: {
				if(subcommand1 == 2)
				{
					serverSend(user.PairUser->Socket, buffer);
					user.isInGame = false;
					user.PairUser->isInGame = false;
				}
		}
			break;
		case 9:

			break;
		}
		memset(buffer, '\0', MESSAGE_SIZE); //Очистить буфер
		memset(response, '\0', MESSAGE_SIZE); //Очистить буфер
	}
	return 1;
}



int main() {
	setlocale(0, "");
	WSADATA WSAData; //Данные 
	SOCKET server, client; //Сокеты сервера и клиента
	SOCKADDR_IN serverAddr, clientAddr; //Адреса сокетов
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0); //Создали сервер
	if (server == INVALID_SOCKET) {
		cout << "Socket creation failed with error:" << WSAGetLastError() << endl;
		return -1;
	}

	// Get the local hostname
	char szHostName[255];
	gethostname(szHostName, 255);
	struct hostent* host_entry;
	host_entry = gethostbyname(szHostName);
	char* szLocalIP;
	szLocalIP = inet_ntoa(*(struct in_addr*)*host_entry->h_addr_list);
	printf("IP сервера: %s Порт сервера: %d\n", szLocalIP, SERVER_PORT);

	serverAddr.sin_addr.s_addr = inet_addr(szLocalIP);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	if (bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cout << "Bind function failed with error: " << WSAGetLastError() << endl;
		return -1;
	}

	if (listen(server, 0) == SOCKET_ERROR) { //Если не удалось получить запрос
		cout << "Listen function failed with error:" << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Сервер ожидает подключений..." << endl;

	while (true) {
		int clientAddrSize = sizeof(clientAddr); //Инициализировать адерс клиента
		if ((client = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
			//Если соединение установлено
			cout << "Новое подключение!" << endl;

			DWORD tid; //Идентификатор
			HANDLE t1 = CreateThread(NULL, 0, serverReceive, &client, 0, &tid); //Создание потока для получения данных
			if (t1 == NULL) { //Ошибка создания потока
				cout << "Thread Creation Error: " << WSAGetLastError() << endl;
			}

		}
	}
}