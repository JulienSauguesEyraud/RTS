#ifndef _VEGETATION_H
#define _VEGETATION_H

#pragma once

#include "Rabbit.h"

class Vegetation
{
protected:
	int x;
	int y;
	int amount = 1500000;
	int maxAmount = 3000000;

	void grow();
	void propagate(std::vector<Vegetation>& vegetations, int gridWidth, int gridHeight);

public:
	bool canReproduce;

	Vegetation(std::vector<Vegetation> vegetations, int gridWidth, int gridHeight, int initialX, int initialY);
	virtual ~Vegetation();
	int getX() const;
	int getY() const;
	float getPercentageAmount() const;
	bool canBeEaten(int amountEaten) const;
	void update(std::vector<Vegetation>& vegetations, int gridWidth, int gridHeight);
	void reduceVegetation(int amountEaten);
};

#endif
