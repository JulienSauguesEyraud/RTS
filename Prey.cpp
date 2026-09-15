#include "Prey.h"

Prey::Prey(int gridWidth, int gridHeight, bool initialPrey, int initialX, int initialY)
{
	if (initialX >= 0 && initialY >= 0)
	{
		x = initialX;
		y = initialY;
	}
	else
	{
		x = QRandomGenerator::global()->bounded(gridWidth);
		y = QRandomGenerator::global()->bounded(gridHeight);
	}
	if (initialPrey)
	{
		age = minimumAgeForReproduction;
		canReproduce = true;
	}
	else
	{
		age = 0;
		canReproduce = false;
	}
	lastReproduction = daysToReproduce;
}

int Prey::getX() const
{
	return x;
}

int Prey::getY() const
{
	return y;
}

void Prey::update(int gridWidth, int gridHeight, int deltaT, int currentTime)
{
	move(gridWidth, gridHeight, deltaT);
	grow(deltaT, currentTime);
}

void Prey::move(int gridWidth, int gridHeight, int deltaT)
{
	for (int i = 0; i < deltaT; ++i)
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
}

void Prey::grow(int deltaT, int currentTime)
{
	if (!canReproduce)
	{
		if (age >= minimumAgeForReproduction)
		{
			if (currentTime - lastReproduction >= daysToReproduce)
			{
				canReproduce = true;
			}
		}
	}
	age += deltaT;
}

void Prey::reproduce(std::vector<Prey>& preys, int gridWidth, int gridHeight) const
{
	// L'enfant apparait sur la même case que les parents
	preys.emplace_back(gridWidth, gridHeight, false, x, y);

	// L'enfant apparait sur une case aléatoire
	// preys.emplace_back(gridWidth, gridHeight, false);
}

bool Prey::IsChidren() const
{
	return age < minimumAgeForReproduction;
}

void Prey::setLastReproduction(int currentTime)
{
	lastReproduction = currentTime;
}
