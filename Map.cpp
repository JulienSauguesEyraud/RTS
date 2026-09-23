#include <Map.h>

#include <QPainter>
#include <QThread>
#include <fstream>

Map::Map(QWidget *parent):
    QWidget(parent)
{
    for (int i = 0; i < H0; ++i)
    {
        //preys.emplace_back(N, N);
		predators.emplace_back(N, N);
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
	//DrawPreys(painter, cellWidth, cellHeight);
	DrawPredators(painter, cellWidth, cellHeight);

	painter.setFont(QFont("Arial", 12));
	//painter.drawText(0, 12, QString("Proies: " + QString::number(preys.size())));
    painter.drawText(0, 36, QString("Prédateurs: " + QString::number(predators.size())));
	painter.drawText(0, 54, QString("Jour: " + QString::number(currentTime)));
}

void Map::start()
{
    QThread *thread = QThread::create([this]()
    {
        std::ofstream file("evolution_poppulations.csv");
        file << "Temps;Proies;Predateurs\n";

        while (true)
        {
            QThread::sleep(deltaT);

            //updatePreys();
			updatePredators();
            //reproducePreys();
			//reproducePredators();
			FightPredators();

            currentTime += deltaT;

            file << currentTime << ";" << /*preys.size() << */ ";" << predators.size() << "\n";
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

//void Map::DrawPreys(QPainter& painter, const int& cellWidth, const int& cellHeight)
//{
//    for (const auto& prey : preys) {
//        if (prey.IsChidren())
//        {
//            painter.setBrush(Qt::cyan);
//        }
//        else if (!prey.canReproduce)
//        {
//            painter.setBrush(Qt::darkCyan);
//        }
//        else
//        {
//			painter.setBrush(Qt::blue);
//        }
//
//        painter.drawEllipse(prey.getX() * cellWidth, prey.getY() * cellHeight, cellWidth, cellHeight);
//    }
//}

void Map::DrawPredators(QPainter& painter, const int& cellWidth, const int& cellHeight)
{
    for (const auto& predator : predators) {
        if (predator.IsChidren())
        {
            painter.setBrush(Qt::yellow);
        }
        else if (!predator.canReproduce)
        {
            painter.setBrush(Qt::darkYellow);
        }
        else
        {
            painter.setBrush(Qt::red);
        }

        painter.drawEllipse(predator.getX() * cellWidth, predator.getY() * cellHeight, cellWidth, cellHeight);
    }
}

//void Map::updatePreys()
//{
//    for (auto& prey : preys) {
//        prey.update(N, N, deltaT, currentTime);
//    }
//}

void Map::updatePredators()
{
    for (auto& predator : predators) {
        predator.update(N, N, deltaT, currentTime);
    }
}

//void Map::reproducePreys()
//{
//	std::vector<Prey> newPreys;
//	for (int i = 0; i < preys.size(); ++i)
//	{
//		if (!preys[i].canReproduce) continue;
//
//		for (int j = i + 1; j < preys.size(); ++j)
//		{
//            if (preys[j].canReproduce)
//			{
//                if (preys[i].getX() == preys[j].getX() && preys[i].getY() == preys[j].getY())
//				{
//					preys[i].reproduce(newPreys, N, N);
//					preys[i].canReproduce = false;
//					preys[j].canReproduce = false;
//					preys[i].setLastReproduction(currentTime);
//					preys[j].setLastReproduction(currentTime);
//                    break;
//				}
//			}
//		}
//	}
//
//	for (const auto& newPrey : newPreys)
//	{
//		preys.push_back(newPrey);
//	}
//}

void Map::reproducePredators()
{
    std::vector<Predator> newPredators;
    for (int i = 0; i < predators.size(); ++i)
    {
        if (!predators[i].canReproduce) continue;

        for (int j = i + 1; j < predators.size(); ++j)
        {
            if (predators[j].canReproduce)
            {
                if (predators[i].getX() == predators[j].getX() && predators[i].getY() == predators[j].getY())
                {
                    predators[i].reproduce(newPredators, N, N);
                    predators[i].canReproduce = false;
                    predators[j].canReproduce = false;
                    predators[i].setLastReproduction(currentTime);
                    predators[j].setLastReproduction(currentTime);
                    break;
                }
            }
        }
    }

    for (const auto& newPredator : newPredators)
    {
        predators.push_back(newPredator);
    }
}

void Map::FightPredators()
{
    std::vector<bool> dead(predators.size(), false);

    for (int i = 0; i < predators.size(); ++i)
    {
        if (dead[i]) continue;

        for (int j = i + 1; j < predators.size(); ++j)
        {
            if (dead[j]) continue;

            if (predators[i].getX() == predators[j].getX() && predators[i].getY() == predators[j].getY())
            {
                if (QRandomGenerator::global()->bounded(2) == 0)
                {
                    dead[i] = true;
                    break; 
                }
                else
                {
                    dead[j] = true; 
                }
            }
        }
    }

    std::vector<Predator> survivors;
    for (size_t i = 0; i < predators.size(); ++i)
    {
        if (!dead[i])
        {
            survivors.push_back(predators[i]);
        }
    }
    predators = std::move(survivors);
}
