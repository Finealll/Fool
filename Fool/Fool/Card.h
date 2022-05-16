#pragma once
#include "CardPreset.h"
#include "Window.h"

#define CARD_WIDTH 140
#define CARD_HEIGHT 189

class Card : public Window
{
	CardPreset* _cardPreset;

public:
	HMENU hMenu;

	Card(Application* app, CardPreset* cardPreset, HWND parent, HMENU hMenu);

	void ShirtUp();
	void CardUp();
};


