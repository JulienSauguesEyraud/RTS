#include "Vegetation.h"
#include <QRandomGenerator>

void Vegetation::grow()
{
	amount += 50000;
	if (amount > maxAmount)
	{
		amount = maxAmount;
	}
}

void Vegetation::propagate(std::vector<Vegetation>& vegetations, int gridWidth, int gridHeight)
{
	if (amount > maxAmount * 0.75 && QRandomGenerator::global()->bounded(100) == 0)
	{
		int direction = QRandomGenerator::global()->bounded(4);
		int newX = x;
		int newY = y;
		switch (direction)
		{
		case 0: // Haut
			newY = (newY + 1) % gridHeight;
			break;
		case 1: // Bas
			newY = (newY - 1 + gridHeight) % gridHeight;
			break;
		case 2: // Droite
			newX = (newX + 1) % gridWidth;
			break;
		case 3: // Gauche
			newX = (newX - 1 + gridWidth) % gridWidth;
			break;
		}

		for (const auto& vegetation : vegetations)
		{
			if (vegetation.getX() == newX && vegetation.getY() == newY)
			{
				return; // Il y a déjà de la végétation à cet endroit
			}
		}
		vegetations.emplace_back(vegetations, gridWidth, gridHeight, newX, newY);
	}
}

Vegetation::Vegetation(std::vector<Vegetation> vegetations, int gridWidth, int gridHeight, int initialX, int initialY)
{
	x = initialX;
	y = initialY;
}

Vegetation::~Vegetation()
{
}

int Vegetation::getX() const
{
	return x;
}

int Vegetation::getY() const
{
	return y;
}

float Vegetation::getPercentageAmount() const
{
	return static_cast<float>(amount) / static_cast<float>(maxAmount);
}

bool Vegetation::canBeEaten(int amountEaten) const
{
	return amountEaten <= amount;
}

void Vegetation::update(std::vector<Vegetation>& vegetations, int gridWidth, int gridHeight)
{
	grow();
	propagate(vegetations, gridWidth, gridHeight);
}

void Vegetation::reduceVegetation(int amountEaten)
{
	if (!canBeEaten(amountEaten))
	{
		return;
	}
	amount -= amountEaten;
}