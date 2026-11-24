#include "Rabbit.h"

Rabbit::Rabbit(int gridWidth, int gridHeight, bool isDead, int x, int y)
{
	daysToReproduce = QRandomGenerator::global()->bounded(73, 122);
	minimumAgeForReproduction = QRandomGenerator::global()->bounded(90, 120);
	ageToDie = QRandomGenerator::global()->bounded(2200, 3300);
	Creature::Creature(gridWidth, gridHeight, isDead, x, y);
}

void Rabbit::reproduce(std::vector<Rabbit>& newRabbits, int gridWidth, int gridHeight) const
{
	int nbChildren = QRandomGenerator::global()->bounded(3, 7);
	for (int i = 0; i < nbChildren; ++i) // Chaque couple de renards peut avoir jusqu'à 2 enfants
	{
		// L'enfant apparait sur la même case que les parents
		newRabbits.emplace_back(gridWidth, gridHeight, false, x, y);
	}

	// L'enfant apparait sur une case aléatoire
	// newRabbits.emplace_back(gridWidth, gridHeight, false);
}
