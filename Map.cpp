#include <Map.h>

#include <QPainter>
#include <QThread>

Map::Map(QWidget *parent):
    QWidget(parent)
{
    for (int i = 0; i < H0; ++i)
    {
        preys.emplace_back(N, N);
    }
}

Map::~Map()
{
}

QSize Map::sizeHint() const
{
    // TODO
    return QSize(600, 600);
}

QSize Map::minimumSizeHint() const
{
    return sizeHint();
}

void Map::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    const int cellWidth = width() / N;
    const int cellHeight = height() / N;

	DrawGrid(painter, cellWidth, cellHeight);
	DrawPreys(painter, cellWidth, cellHeight);
}

void Map::start()
{
    QThread *thread = QThread::create([this]()
    {
        // TODO
    });

    thread->start();
}

// 3.1.1
void Map::DrawGrid(QPainter& painter, const int& cellWidth, const int& cellHeight)
{
    for (int row = 0; row < N; ++row)
    {
        for (int column = 0; column < N; ++column)
        {
            painter.drawRect(
                column * cellWidth,
                row * cellHeight,
                cellWidth,
                cellHeight
            );
        }
    }
}

//3.1.2
void Map::DrawPreys(QPainter& painter, const int& cellWidth, const int& cellHeight)
{
    painter.setBrush(Qt::blue);
    for (const auto& prey : preys) {
        painter.drawEllipse(prey.getX() * cellWidth, prey.getY() * cellHeight, cellWidth, cellHeight);
    }
}