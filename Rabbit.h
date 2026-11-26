#ifndef _RABBIT_H
#define _RABBIT_H

#pragma once

#include <QRandomGenerator>
#include <vector>
#include <memory>

#include "Creature.h"

class Rabbit : public Creature
{
protected:
	int amoutVegetationToEat = 5;

	virtual void grow(int deltaT, int currentTime) override;
public:
	Rabbit(int gridWidth, int gridHeight, bool initialCreature = true, int x = -1, int y = -1);
	void reproduce(std::vector<Rabbit>& newRabbits, int gridWidth, int gridHeight) const;
	int getAmoutVegetationToEat();
};

#endif
