#pragma once
#include "Extensions.h"

#define MESSAGE_SIZE 128

char* GetCommandForStart()
{
	std::vector<int> cards;
	char mess[MESSAGE_SIZE];
	// Отправка данных о старте игры
	mess[0] = STARTGAME;
	mess[1] = START;

	GetCards(cards);
	ShuffleCards(cards);

	int offset = 4;
	for(int i = 0; i < cards.size(); i++)
	{
		mess[i + offset] = cards[i];
	}
	return mess;
}