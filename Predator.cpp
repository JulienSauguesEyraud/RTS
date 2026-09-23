#include "Predator.h"

void Predator::reproduce(std::vector<Predator>& newPredators, int gridWidth, int gridHeight) const
{
	// L'enfant apparait sur la même case que les parents
	newPredators.emplace_back(gridWidth, gridHeight, false, x, y);

	// L'enfant apparait sur une case aléatoire
	// newPredators.emplace_back(gridWidth, gridHeight, false);
}
