#pragma once
#include "CardPreset.h"
#include "Window.h"

#define CARD_WIDTH 140
#define CARD_HEIGHT 189

class Card : public Window
{
	CardPreset* _cardPreset;

	HMENU _hMenu;
public:

	Card(Application* app, CardPreset* cardPreset, HWND parent, HMENU hMenu);
};


