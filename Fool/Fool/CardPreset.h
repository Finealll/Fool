#pragma once
#include "resource.h"

enum CARD_SUIT {SHIRT = 0, HEARTS = 1, DIAMONDS = 2, CLUBS = 3, SPADES = 4};

struct CardPreset
{
	int CardDefineId;
	int CardNumber;
	int SystemNumber;;
	CARD_SUIT CardSuit;
};

CardPreset* GetCardPresets();