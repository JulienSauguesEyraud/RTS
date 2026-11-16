#ifndef _FOX_H
#define _FOX_H

#pragma once

#include <QRandomGenerator>
#include <vector>
#include <memory>

#include "Creature.h"

class Fox : public Creature
{
protected:
	int satiete = 10;
	bool isDead = false;
	void grow(int deltaT, int currentTime) override;
public:
	using Creature::Creature;
	bool isHungry() const;
	bool isVeryHungry() const;
	void resetSatiete();
	void reproduce(std::vector<Fox>& newFoxes, int gridWidth, int gridHeight) const;
	bool isDeadStatus() const;
};

#endif