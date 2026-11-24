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

Fox::Fox(int gridWidth, int gridHeight, bool isDead, int x, int y)
{
	daysToReproduce = 365;
	minimumAgeForReproduction = 300;
	ageToDie = QRandomGenerator::global()->bounded(2920, 3650);
	Creature::Creature(gridWidth, gridHeight, isDead, x, y);
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
	int nbChildren = QRandomGenerator::global()->bounded(4, 6);
	for (int i = 0; i < nbChildren; ++i) // Chaque couple de renards peut avoir jusqu'à 2 enfants
	{
		// L'enfant apparait sur la même case que les parents
		newFoxes.emplace_back(gridWidth, gridHeight, false, x, y);
	}

	// L'enfant apparait sur une case aléatoire
	// newFoxes.emplace_back(gridWidth, gridHeight, false);
}
