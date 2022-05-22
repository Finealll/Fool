﻿#include "CardPreset.h"

CardPreset* GetCardPresets()
{
	CardPreset* dtos = new CardPreset[]
	{
		{IDB_BITMAP1, 6, 1,DIAMONDS},
		{IDB_BITMAP2, 6, 2,HEARTS},
		{IDB_BITMAP3, 6, 3,CLUBS},
		{IDB_BITMAP4, 6, 4,SPADES},
		{IDB_BITMAP5, 7, 5,DIAMONDS},
		{IDB_BITMAP6, 7, 6,HEARTS},
		{IDB_BITMAP7, 7, 7,CLUBS},
		{IDB_BITMAP8, 7, 8,SPADES},
		{IDB_BITMAP9, 8, 9,DIAMONDS},
		{IDB_BITMAP10, 8, 10,HEARTS},
		{IDB_BITMAP11, 8, 11,CLUBS},
		{IDB_BITMAP12, 8, 12,SPADES},
		{IDB_BITMAP13, 9, 13,DIAMONDS},
		{IDB_BITMAP14, 9, 14,HEARTS},
		{IDB_BITMAP15, 9, 15,CLUBS},
		{IDB_BITMAP16, 9, 16,SPADES},
		{IDB_BITMAP17, 10, 17, DIAMONDS},
		{IDB_BITMAP18, 10, 18, HEARTS},
		{IDB_BITMAP19, 10, 19, CLUBS},
		{IDB_BITMAP20, 10, 20, SPADES},
		{IDB_BITMAP21, 11, 21, DIAMONDS},
		{IDB_BITMAP22, 11, 22, HEARTS},
		{IDB_BITMAP23, 11, 23, CLUBS},
		{IDB_BITMAP24, 11, 24, SPADES},
		{IDB_BITMAP25, 12, 25, DIAMONDS},
		{IDB_BITMAP26, 12, 26, HEARTS},
		{IDB_BITMAP27, 12, 27, CLUBS},
		{IDB_BITMAP28, 12, 28, SPADES},
		{IDB_BITMAP29, 13, 29, DIAMONDS},
		{IDB_BITMAP30, 13, 30, HEARTS},
		{IDB_BITMAP31, 13, 31, CLUBS},
		{IDB_BITMAP32, 13, 32, SPADES},
		{IDB_BITMAP33, 14, 33, DIAMONDS},
		{IDB_BITMAP34, 14, 34, HEARTS},
		{IDB_BITMAP35, 14, 35, CLUBS},
		{IDB_BITMAP36, 14, 36, SPADES},
		{IDB_BITMAP37, 0, 0, SHIRT}
	};
	return dtos;
}
