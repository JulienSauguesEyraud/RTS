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

void Prey::update(int gridWidth, int gridHeight, int deltaT, std::vector<Prey>& preys)
{
	move(gridWidth, gridHeight);
	grow(deltaT);
	reproduce(preys, gridWidth, gridHeight);
}

// 3.1.3.1
void Prey::move(int gridWidth, int gridHeight)
{
	int direction = QRandomGenerator::global()->bounded(4);
	switch (direction)
	{
		case 0: // Haut
			y = (y + 1) % gridHeight;
			break;
		case 1: // Bas
			y = (y - 1 + gridHeight) % gridHeight;
			break;
		case 2: // Droite
			x = (x + 1) % gridWidth;
			break;
		case 3: // Gauche
			x = (x - 1 + gridWidth) % gridWidth;
			break;
	}
}

// 3.1.3.2
void Prey::grow(int deltaT)
{
	age += deltaT;
}

// 3.1.3.3
void Prey::reproduce(std::vector<Prey>& preys, int gridWidth, int gridHeight) const
{
	if (QRandomGenerator::global()->bounded(1.0) < pReproduction)
	{
		preys.push_back(Prey(gridWidth, gridHeight));
	}
}