#ifndef _MAP_H
#define _MAP_H

#include <QWidget>
#include <vector>
#include <memory>
#include "Prey.h"
#include "Predator.h"

class Map: public QWidget
{
private:
    const int N = 20;
    const int H0 = 50;
	const int deltaT = 1;
    int currentTime = 0;

    std::vector<Prey> preys;
    std::vector<Predator> predators;

    void DrawGrid(QPainter& painter, const int& cellWidth, const int& cellHeight) const;
    void DrawPreys(QPainter& painter, const int& cellWidth, const int& cellHeight);
    void DrawPredators(QPainter& painter, const int& cellWidth, const int& cellHeight);
    void updatePreys();
	void updatePredators();
	void reproducePreys();
	// void reproducePredators();
    void FightPredators();
    void PredatorsEatPreys();

public:
    Map(QWidget *parent = nullptr);
    virtual ~Map();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void paintEvent(QPaintEvent *event) override;

    void start();

};

#endif
