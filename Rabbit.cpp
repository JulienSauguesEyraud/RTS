#include "Rabbit.h"

void Rabbit::grow(int deltaT, int currentTime)
{
	Creature::grow(deltaT, currentTime);
	if (!isChidren())
	{
		amoutVegetationToEat = 300;
	}
}

Rabbit::Rabbit(int gridWidth, int gridHeight, bool initialCreature, int x, int y)
	: Creature(gridWidth, gridHeight,
		QRandomGenerator::global()->bounded(90, 121),
		QRandomGenerator::global()->bounded(73, 122),
		QRandomGenerator::global()->bounded(2200, 3300),
		initialCreature, x, y)
{
	if (initialCreature)
	{
		amoutVegetationToEat = 300;
	}
	else
	{
		amoutVegetationToEat = 150;
	}
}

void Rabbit::reproduce(std::vector<Rabbit>& newRabbits, int gridWidth, int gridHeight) const
{
	int nbChildren = QRandomGenerator::global()->bounded(3, 8);
	for (int i = 0; i < nbChildren; ++i)
	{
		// L'enfant apparait sur la même case que les parents
		newRabbits.emplace_back(gridWidth, gridHeight, false, x, y);
	}

	// L'enfant apparait sur une case aléatoire
	// newRabbits.emplace_back(gridWidth, gridHeight, false);
}

int Rabbit::getAmoutVegetationToEat()
{
	return amoutVegetationToEat;
}
