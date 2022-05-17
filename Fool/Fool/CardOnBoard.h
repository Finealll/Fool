#pragma once
#include "Card.h"

class GameWindow;
class CardOnBoard
{
private:
	Card* _card;
	Card* _hittingCard;
	CARD_SUIT _trump;

	GameWindow* _gameWindow;

	HWND _hitPreviewHwnd;			// Возможность хода!!!!!!!!!!!
public:

	CardOnBoard(GameWindow* gameWindow, CARD_SUIT trump, Card* settedCard);

	void SetCard(Card* card) { _card = card; }
	void SetHittingCard(Card* hittingCard) { _hittingCard = hittingCard; }
	void SetTrump(CARD_SUIT trump) { _trump = trump; }

	Card* GetCard() { return _card; }
	Card* GetHittingCard() { return _hittingCard; }

	void ClearCards();

	bool CheckPossibilityToHit(Card* card);

	void MovePosition(int posX, int posY);

	void Show(bool IsShow = true);
};

