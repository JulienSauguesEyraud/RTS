#include "Prey.h"

Prey::Prey(int gridWidth, int gridHeight)
{
	x = QRandomGenerator::global()->bounded(gridWidth);
	y = QRandomGenerator::global()->bounded(gridHeight);
}

int Prey::getX() const
{
	return x;
}

int Prey::getY() const
{
	return y;
}
