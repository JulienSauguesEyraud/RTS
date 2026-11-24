#include "Fox.h"

void Fox::grow(int deltaT, int currentTime)
{
	Creature::grow(deltaT, currentTime);
	satiete--;
	if (satiete <= 0)
	{
		isDead = true;
	}
}

bool Fox::isHungry() const
{
	return satiete <= 5;
}

bool Fox::isVeryHungry() const
{
	return satiete <= 2;
}

int Fox::getSatiete() const
{
	return satiete;
}

void Fox::reproduce(std::vector<Fox>& newFoxes, int gridWidth, int gridHeight) const
{
	// L'enfant apparait sur la même case que les parents
	newFoxes.emplace_back(gridWidth, gridHeight, false, x, y);

	// L'enfant apparait sur une case aléatoire
	// newFoxes.emplace_back(gridWidth, gridHeight, false);
}
