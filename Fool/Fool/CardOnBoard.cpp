#include "CardOnBoard.h"

#include "GameWindow.h"

CardOnBoard::CardOnBoard(GameWindow* gameWindow, CARD_SUIT trump, Card* settedCard, HWND cardHitHwnd)
{
	_gameWindow = gameWindow;
	_trump = trump;
	_card = settedCard;

	_hitPreviewHwnd = cardHitHwnd;
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
	if(_hittingCard != nullptr)
	{
		retValue = false;
	}
	// Если карта, которую бьем - козырь
	else if (isCardTrump) 
	{
		if(isHittedCardTrump && 
			(cardPreset->CardNumber < hittedCardPreset->CardNumber))
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
			(cardPreset->CardNumber < hittedCardPreset->CardNumber))
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

void CardOnBoard::CreateHitPrevies(Card* card, bool IsShow)
{
	ShowWindow(_hitPreviewHwnd, IsShow);
	_hittingCardPreview = card;
}

void CardOnBoard::DeleteHitPrevies()
{
	ShowWindow(_hitPreviewHwnd, false);
	_hittingCardPreview = nullptr;
}
