#ifndef _FOX_H
#define _FOX_H

#pragma once

#include <QRandomGenerator>
#include <vector>
#include <memory>

#include "Creature.h"

class Fox : public Creature
{
public:
	Fox(int gridWidth, int gridHeight, bool initialCreature = true, int x = -1, int y = -1);
	bool isHungry() const;
	bool isVeryHungry() const;
	int getSatiete() const;
	void reproduce(std::vector<Fox>& newFoxes, int gridWidth, int gridHeight) const;
};

#endif
