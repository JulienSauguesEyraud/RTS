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
	void grow(int deltaT, int currentTime) override;
public:
	Fox(int gridWidth, int gridHeight, bool isDead, int x, int y);
	bool isHungry() const;
	bool isVeryHungry() const;
	int getSatiete() const;
	void reproduce(std::vector<Fox>& newFoxes, int gridWidth, int gridHeight) const;
};

#endif