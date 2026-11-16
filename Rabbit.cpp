#include "Rabbit.h"

void Rabbit::reproduce(std::vector<Rabbit>& newRabbits, int gridWidth, int gridHeight) const
{
	// L'enfant apparait sur la même case que les parents
	// newRabbits.emplace_back(gridWidth, gridHeight, false, x, y);

	// L'enfant apparait sur une case aléatoire
	newRabbits.emplace_back(gridWidth, gridHeight, false);
}
