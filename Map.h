#ifndef _MAP_H
#define _MAP_H

#include <QWidget>
#include <QImage>
#include <vector>
#include <memory>
#include "Rabbit.h"
#include "Fox.h"
#include "Vegetation.h"

class Map: public QWidget
{
private:
    QImage rabbitImage;
    QImage foxImage;
    const bool showImages = true;
    const int N = 20;
    const int H0Rabbit = 200;
	const int H0Fox = 20;
	const int H0Vegetation = 50;
	const int deltaT = 1;
    int currentTime = 0;

    std::vector<Rabbit> rabbits;
    std::vector<Fox> foxes;
	std::vector<Vegetation> vegetations;

    void drawGrid(QPainter& painter, const int& cellWidth, const int& cellHeight) const;
    void drawRabbits(QPainter& painter, const int& cellWidth, const int& cellHeight);
    void drawFoxes(QPainter& painter, const int& cellWidth, const int& cellHeight);
	void drawVegetation(QPainter& painter, const int& cellWidth, const int& cellHeight);
    void updateRabbits();
	void updateFoxes();
	void updateVegetation();
	void reproduceRabbits();
	void rabbitsEatVegetation();
	void checkFightOrReproduceFoxes();
	void reproduceFoxes(int i, int j, std::vector<Fox>& newFoxes, std::vector<bool>& reproduced);
    void fightFoxes(int i, int j, std::vector<bool>& dead);
    void foxesEatRabbits();
	void removeDeadFoxes();
	void removeDeadRabbits();

public:
    Map(QWidget *parent = nullptr);
    virtual ~Map();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void paintEvent(QPaintEvent *event) override;

    void start();

};

#endif
