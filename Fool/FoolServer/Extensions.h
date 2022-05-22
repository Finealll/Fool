#pragma once
#include <random>
#include <vector>

void GetCards(std::vector<int>& cards)
{
	for (int i = 1; i <= 36; i++)
		cards.push_back(i);
}


void ShuffleCards(std::vector<int>& cards)
{
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(cards.begin(), cards.end(), g);
}
