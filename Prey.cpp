#include "Prey.h"

void Prey::reproduce(std::vector<std::shared_ptr<Creature>>& newCreatures, int gridWidth, int gridHeight) const
{
	// L'enfant apparait sur la même case que les parents
	newCreatures.push_back(std::make_shared<Prey>(gridWidth, gridHeight, false, x, y));

	// L'enfant apparait sur une case aléatoire
	// newCreatures.push_back(std::make_shared<Prey>(gridWidth, gridHeight, false));
} 
