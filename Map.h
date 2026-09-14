#ifndef _MAP_H
#define _MAP_H

#include <QWidget>
#include <vector>
#include "Prey.h"

class Map: public QWidget
{
private:
    const int N = 20;
    const int H0 = 10;
	const int deltaT = 1;

    std::vector<Prey> preys;

public:
    Map(QWidget *parent = nullptr);
    virtual ~Map();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void paintEvent(QPaintEvent *event) override;

    void start();

	void DrawGrid(QPainter& painter, const int& cellWidth, const int& cellHeight) const;
    void DrawPreys(QPainter& painter, const int& cellWidth, const int& cellHeight);
	void updatePreys();
};

#endif
