#pragma once
#include "Card.h"

class GameWindow;
class CardOnBoard
{
private:
	Card* _card;
	Card* _hittingCard;
	Card* _hittingCardPreview;
	CARD_SUIT _trump;

	GameWindow* _gameWindow;

	HWND _hitPreviewHwnd;	
public:

	CardOnBoard(GameWindow* gameWindow, CARD_SUIT trump, Card* settedCard, HWND cardHitHwnd);

	void SetCard(Card* card) { _card = card; }
	void SetHittingCard(Card* hittingCard) { _hittingCard = hittingCard; }
	void SetTrump(CARD_SUIT trump) { _trump = trump; }

	Card* GetCard() { return _card; }
	Card* GetHittingCard() { return _hittingCard; }
	Card* GetHittingCardPreview() { return _hittingCardPreview; }

	bool IsHitted() { return _hittingCard != nullptr; }

	void ClearCards();

	bool CheckPossibilityToHit(Card* card);

	void MovePosition(int posX, int posY);

	void Show(bool IsShow = true);
	void CreateHitPrevies(Card* card, bool IsShow = true);
	void DeleteHitPrevies();
};

