#include "Creature.h"

Creature::Creature(int gridWidth, int gridHeight,
	int minimumAgeForReproduction, int daysToReproduce, int ageToDie,
	bool initialCreature, int initialX, int initialY)
	: minimumAgeForReproduction(minimumAgeForReproduction),
	  daysToReproduce(daysToReproduce),
	  ageToDie(ageToDie)
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
	if (initialCreature)
	{
		age = this->minimumAgeForReproduction;
		canReproduce = true;
	}
	else
	{
		age = 0;
		canReproduce = false;
	}
	lastReproduction = -this->daysToReproduce;
	isMale = QRandomGenerator::global()->bounded(2) == 0;
}

Creature::~Creature()
{
}

int Creature::getX() const
{
	return x;
}

int Creature::getY() const
{
	return y;
}

bool Creature::getIsMale() const
{
	return isMale;
}

void Creature::update(int gridWidth, int gridHeight, int deltaT, int currentTime)
{
	move(gridWidth, gridHeight);
	grow(deltaT, currentTime);
}

void Creature::move(int gridWidth, int gridHeight)
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

void Creature::grow(int deltaT, int currentTime)
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

	if (age >= ageToDie)
	{
		isDead = true;
	}
}

bool Creature::isChidren() const
{
	return age < minimumAgeForReproduction;
}

void Creature::setLastReproduction(int currentTime)
{
	lastReproduction = currentTime;
}

void Creature::resetSatiete()
{
	satiete = 10;
	isDead = false;
}

bool Creature::isDeadStatus() const
{
	return isDead;
}
