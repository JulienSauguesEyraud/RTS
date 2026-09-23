#include "Prey.h"

void Prey::reproduce(std::vector<Prey>& newPreys, int gridWidth, int gridHeight) const
{
	// L'enfant apparait sur la même case que les parents
	newPreys.emplace_back(gridWidth, gridHeight, false, x, y);

	// L'enfant apparait sur une case aléatoire
	// newPreys.emplace_back(gridWidth, gridHeight, false);
}
