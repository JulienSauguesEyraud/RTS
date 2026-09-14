#ifndef _PREY_H
#define _PREY_H

#pragma once

#include <QRandomGenerator>

class Prey
{
private:
	int x;
	int y;

public:
	Prey(int gridWidth, int gridHeight);
	int getX() const;
	int getY() const;
};

#endif