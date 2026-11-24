#ifndef _MAP_H
#define _MAP_H

#include <QWidget>
#include <QImage>
#include <vector>
#include <memory>
#include "Rabbit.h"
#include "Fox.h"

class Map: public QWidget
{
private:
    QImage rabbitImage;
    QImage foxImage;
    const bool showImages = true;
    const int N = 20;
    const int H0Rabbit = 200;
	const int H0Fox = 20;
	const int deltaT = 1;
    int currentTime = 0;

    std::vector<Rabbit> rabbits;
    std::vector<Fox> foxes;

    void DrawGrid(QPainter& painter, const int& cellWidth, const int& cellHeight) const;
    void DrawRabbits(QPainter& painter, const int& cellWidth, const int& cellHeight);
    void DrawFoxes(QPainter& painter, const int& cellWidth, const int& cellHeight);
    void updateRabbits();
	void updateFoxes();
	void reproduceRabbits();
	void CheckFightOrReproduceFoxes();
	void reproduceFoxes(int i, int j, std::vector<Fox>& newFoxes, std::vector<bool>& reproduced);
    void FightFoxes(int i, int j, std::vector<bool>& dead);
    void FoxesEatRabbits();
	void RemoveDeadFoxes();

public:
    Map(QWidget *parent = nullptr);
    virtual ~Map();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void paintEvent(QPaintEvent *event) override;

    void start();

};

#endif
