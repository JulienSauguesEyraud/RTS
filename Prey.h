#ifndef _PREY_H
#define _PREY_H

#pragma once

#include <QRandomGenerator>
#include <vector>

class Prey
{
private:
	int x;
	int y;
	int age = 0;
	int minimumAgeForReproduction = 30;
	int daysToReproduce = 10;
	int lastReproduction;

	void move(int gridWidth, int gridHeight, int deltaT);
	void grow(int deltaT, int currentTime);

public:
	bool canReproduce;

	Prey(int gridWidth, int gridHeight, bool initialPrey = true, int initialX = -1, int initialY = -1);
	int getX() const;
	int getY() const;
	void update(int gridWidth, int gridHeight, int deltaT, int currentTime);
	void reproduce(std::vector<Prey>& preys, int gridWidth, int gridHeight) const;
	bool IsChidren() const;
	void setLastReproduction(int currentTime);
};

#endif