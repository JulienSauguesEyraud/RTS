#ifndef _RABBIT_H
#define _RABBIT_H

#pragma once

#include <QRandomGenerator>
#include <vector>
#include <memory>

#include "Creature.h"

class Rabbit : public Creature
{
public:
	Rabbit(int gridWidth, int gridHeight, bool initialCreature = true, int x = -1, int y = -1);
	void reproduce(std::vector<Rabbit>& newRabbits, int gridWidth, int gridHeight) const;
};

#endif
