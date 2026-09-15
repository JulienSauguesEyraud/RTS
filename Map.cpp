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
	painter.drawText(0, 36, QString("Jour: " + QString::number(currentTime)));
}

void Map::start()
{
    QThread *thread = QThread::create([this]()
    {
        std::ofstream file("evolution_preys.csv");
        file << "Temps;Population\n";

        while (true)
        {
            QThread::sleep(deltaT);

            updatePreys();
            reproducePreys();

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

void Map::DrawPreys(QPainter& painter, const int& cellWidth, const int& cellHeight)
{
    for (const auto& prey : preys) {
        if (prey.IsChidren())
        {
            painter.setBrush(Qt::cyan);
        }
        else if (!prey.canReproduce)
        {
            painter.setBrush(Qt::darkCyan);
        }
        else
        {
			painter.setBrush(Qt::blue);
        }

        painter.drawEllipse(prey.getX() * cellWidth, prey.getY() * cellHeight, cellWidth, cellHeight);
    }
}

void Map::updatePreys()
{
    for (auto& prey : preys) {
        prey.update(N, N, deltaT, currentTime);
    }
}

void Map::reproducePreys()
{
	std::vector<Prey> newPreys;
	for (int i = 0; i < preys.size(); ++i)
	{
		if (!preys[i].canReproduce) continue;

		for (int j = i + 1; j < preys.size(); ++j)
		{
            if (preys[j].canReproduce)
			{
                if (preys[i].getX() == preys[j].getX() && preys[i].getY() == preys[j].getY())
				{
					preys[i].reproduce(newPreys, N, N);
					preys[i].canReproduce = false;
					preys[j].canReproduce = false;
					preys[i].setLastReproduction(currentTime);
					preys[j].setLastReproduction(currentTime);
                    break;
				}
			}
		}
	}

	for (const auto& newPrey : newPreys)
	{
		preys.push_back(newPrey);
	}
}
