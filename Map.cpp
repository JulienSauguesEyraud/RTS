#include <Map.h>

#include <QPainter>
#include <QThread>
#include <fstream>

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

	painter.setFont(QFont("Arial", 12));
	painter.drawText(0, 12, QString("Proies: " + QString::number(preys.size())));
}

void Map::start()
{
    QThread *thread = QThread::create([this]()
    {
        std::ofstream file("evolution_preys.csv");
        file << "Temps;Population\n";
        int currentTime = 0;

        while (true)
        {
            QThread::sleep(deltaT);

            updatePreys();

            currentTime += deltaT;

            file << currentTime << ";" << preys.size() << "\n";
            file.flush();


            QMetaObject::invokeMethod(this, [this]() {
                update();
            });
        }
    });

    thread->start();
}

// 3.1.1
void Map::DrawGrid(QPainter& painter, const int& cellWidth, const int& cellHeight) const
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

// 3.1.2
void Map::DrawPreys(QPainter& painter, const int& cellWidth, const int& cellHeight)
{
    painter.setBrush(Qt::blue);
    for (const auto& prey : preys) {
        painter.drawEllipse(prey.getX() * cellWidth, prey.getY() * cellHeight, cellWidth, cellHeight);
    }
}

// 3.1.3.1
void Map::updatePreys()
{
    for (auto& prey : preys) {
        prey.update(N, N, deltaT, preys);
    }
}
