#include "Predator.h"

void Predator::grow(int deltaT, int currentTime)
{
	Creature::grow(deltaT, currentTime);
	satiete--;
}

bool Predator::isHungry() const
{
	return satiete <= 5;
}

void Predator::resetSatiete()
{
	satiete = 10;
}

void Predator::reproduce(std::vector<Predator>& newPredators, int gridWidth, int gridHeight) const
{
	// L'enfant apparait sur la même case que les parents
	newPredators.emplace_back(gridWidth, gridHeight, false, x, y);

	// L'enfant apparait sur une case aléatoire
	// newPredators.emplace_back(gridWidth, gridHeight, false);
}
