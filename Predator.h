#ifndef _PREDATOR_H
#define _PREDATOR_H

#pragma once

#include <QRandomGenerator>
#include <vector>
#include <memory>

#include "Creature.h"

class Predator : public Creature
{
private:
	float p = 0.1f;

public:
	using Creature::Creature;
	void reproduce(std::vector<Predator>& newPredators, int gridWidth, int gridHeight) const;
};

#endif