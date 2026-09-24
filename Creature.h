#ifndef _CREATURE_H
#define _CREATURE_H

#pragma once

#include <QRandomGenerator>
#include <vector>
#include <memory>

class Creature
{
protected:
	int x;
	int y;
	int age = 0;
	int minimumAgeForReproduction = 30;
	int daysToReproduce = 10;
	int lastReproduction;

	void move(int gridWidth, int gridHeight, int deltaT);
	virtual void grow(int deltaT, int currentTime);

public:
	bool canReproduce;

	Creature(int gridWidth, int gridHeight, bool initialCreature = true, int initialX = -1, int initialY = -1);
	virtual ~Creature();
	int getX() const;
	int getY() const;
	virtual void update(int gridWidth, int gridHeight, int deltaT, int currentTime);
	bool IsChidren() const;
	void setLastReproduction(int currentTime);
};

#endif