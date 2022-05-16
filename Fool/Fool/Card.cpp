#include "Card.h"

Card::Card(Application* app, CardPreset* cardPreset, HWND parent, HMENU hMenu) : Window(app)
{
	_cardPreset = cardPreset;
    _hMenu = hMenu;

    this->hwnd = CreateWindowEx(
        0,
        L"BUTTON",
        NULL,
        WS_CHILD | BS_BITMAP,
        0, 0, CARD_WIDTH, CARD_HEIGHT,
        parent,
        hMenu,
        hInstance,
        NULL);

    HBITMAP hbit;
    hbit = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(cardPreset->CardDefineId), IMAGE_BITMAP, 0, 0, 0);

    SendMessage(this->hwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbit);
}