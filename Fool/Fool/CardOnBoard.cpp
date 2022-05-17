#include "CardOnBoard.h"

CardOnBoard::CardOnBoard(GameWindow* gameWindow, CARD_SUIT trump, Card* settedCard)
{
	_gameWindow = gameWindow;
	_trump = trump;
	_card = settedCard;
}

void CardOnBoard::ClearCards()
{
	_card = nullptr;
	_hittingCard = nullptr;
}

bool CardOnBoard::CheckPossibilityToHit(Card* hittedCard)
{
	CardPreset* cardPreset = _card->GetCardPreset();
	CardPreset* hittedCardPreset = hittedCard->GetCardPreset();

	bool isCardTrump = (cardPreset->CardSuit == _trump);
	bool isHittedCardTrump = (hittedCardPreset->CardSuit == _trump);

	bool retValue = false;
	// Если карта, которую бьем - козырь
	if (isCardTrump) 
	{
		if(isHittedCardTrump && 
			(cardPreset->CardNumber < hittedCardPreset->CardDefineId))
		{
			retValue = true;
		} else
		{
			retValue = false;
		}
	} else
	{
		if (isHittedCardTrump)
			retValue = true;
		else if ((cardPreset->CardSuit == hittedCardPreset->CardSuit) &&
			(cardPreset->CardNumber < hittedCardPreset->CardDefineId))
			retValue = true;
		else
			retValue = false;
	}
	return retValue;
}

void CardOnBoard::MovePosition(int posX, int posY)
{
	int xOffset = 20;
	int yOffset = 20;
	_card->MovePositionTop(posX, posY);
	if(_hittingCard != nullptr)
	{
		_hittingCard->MovePositionBottom(posX + xOffset, posY + yOffset);
	}
	SetWindowPos(_hitPreviewHwnd, HWND_BOTTOM, posX + xOffset, posY + yOffset, 0, 0, SWP_NOSIZE);
}

void CardOnBoard::Show(bool IsShow)
{
	_card->Show(IsShow);
	if(_hittingCard != nullptr)
	{
		_hittingCard->Show(IsShow);
	}
}
