#ifndef _PREDATOR_H
#define _PREDATOR_H

#pragma once

#include <QRandomGenerator>
#include <vector>
#include <memory>

#include "Creature.h"

class Predator : public Creature
{
protected:
	int satiete = 10;
	void grow(int deltaT, int currentTime) override;
public:
	using Creature::Creature;
	bool isHungry() const;
	void resetSatiete();
	void reproduce(std::vector<Predator>& newPredators, int gridWidth, int gridHeight) const;
};

#endif