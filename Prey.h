#ifndef _PREY_H
#define _PREY_H

#pragma once

#include <QRandomGenerator>
#include <vector>
#include <memory>

#include "Creature.h"

class Prey : public Creature
{
public:
	using Creature::Creature;
	void reproduce(std::vector<Prey>& newPreys, int gridWidth, int gridHeight) const;
};

#endif