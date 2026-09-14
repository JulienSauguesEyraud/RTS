#ifndef _PREY_H
#define _PREY_H

#pragma once

#include <QRandomGenerator>

class Prey
{
private:
	int x;
	int y;
	int age = 0;
	const float pReproduction = 0.1;

public:
	Prey(int gridWidth, int gridHeight);
	int getX() const;
	int getY() const;
	void update(int gridWidth, int gridHeight, int deltaT, std::vector<Prey>& preys);
	void move(int gridWidth, int gridHeight);
	void grow(int deltaT);
	void reproduce(std::vector<Prey>& preys, int gridWidth, int gridHeight) const;
};

#endif