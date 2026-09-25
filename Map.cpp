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
	DrawPreys(painter, cellWidth, cellHeight);
	DrawPredators(painter, cellWidth, cellHeight);

	painter.setFont(QFont("Arial", 12));
	painter.drawText(0, 12, QString("Proies: " + QString::number(preys.size())));
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
            QThread::sleep(1);

            updatePreys();
			updatePredators();
            PredatorsEatPreys();
            RemoveDeadPredators();
            CheckFightOrReproducePredators();
            reproducePreys();

            currentTime += deltaT;

            file << currentTime << ";" << preys.size() <<  ";" << predators.size() << "\n";
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
            painter.setBrush(Qt::blue);
            painter.drawEllipse(prey.getX() * cellWidth + cellWidth / 4, prey.getY() * cellHeight + cellHeight / 4, cellWidth / 2, cellHeight / 2);
        }
        else if (!prey.canReproduce)
        {
            painter.setBrush(Qt::darkBlue);
            painter.drawEllipse(prey.getX() * cellWidth, prey.getY() * cellHeight, cellWidth, cellHeight);
        }
        else
        {
			painter.setBrush(Qt::blue);
            painter.drawEllipse(prey.getX() * cellWidth, prey.getY() * cellHeight, cellWidth, cellHeight);
        }
    }
}

void Map::DrawPredators(QPainter& painter, const int& cellWidth, const int& cellHeight)
{
    for (const auto& predator : predators) {
        if (predator.IsChidren() && !predator.isHungry())
        {
            painter.setBrush(Qt::red);
            painter.drawEllipse(predator.getX() * cellWidth + cellWidth / 4, predator.getY() * cellHeight + cellHeight / 4, cellWidth / 2, cellHeight / 2);
        }
		else if (predator.IsChidren() && predator.isHungry())
		{
			painter.setBrush(Qt::darkRed);
            painter.drawEllipse(predator.getX() * cellWidth + cellWidth / 4, predator.getY() * cellHeight + cellHeight / 4, cellWidth / 2, cellHeight / 2);
		}
        else if (!predator.canReproduce && !predator.isHungry())
        {
            painter.setBrush(Qt::yellow);
            painter.drawEllipse(predator.getX() * cellWidth, predator.getY() * cellHeight, cellWidth, cellHeight);
        }
        else if (!predator.canReproduce && predator.isHungry())
        {
            painter.setBrush(Qt::darkYellow);
            painter.drawEllipse(predator.getX() * cellWidth, predator.getY() * cellHeight, cellWidth, cellHeight);
        }
        else if (predator.isHungry())
        {
            painter.setBrush(Qt::darkRed);
            painter.drawEllipse(predator.getX() * cellWidth, predator.getY() * cellHeight, cellWidth, cellHeight);
        }
        else
        {
            painter.setBrush(Qt::red);
            painter.drawEllipse(predator.getX() * cellWidth, predator.getY() * cellHeight, cellWidth, cellHeight);
        }
    }
}

void Map::updatePreys()
{
    for (auto& prey : preys) {
        prey.update(N, N, deltaT, currentTime);
    }
}

void Map::updatePredators()
{
    for (auto& predator : predators) {
        predator.update(N, N, deltaT, currentTime);
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

void Map::CheckFightOrReproducePredators()
{
    std::vector<Predator> newPredators;
    std::vector<bool> dead(predators.size(), false);
	std::vector<bool> reproduced(predators.size(), false);

    for (int i = 0; i < predators.size(); ++i)
    {
        if (dead[i]) continue;
		if (reproduced[i]) continue;

        for (int j = i + 1; j < predators.size(); ++j)
        {
            if (predators[i].getX() == predators[j].getX() && predators[i].getY() == predators[j].getY())
            {
				if (dead[j]) continue;
				if (reproduced[j]) continue;

                if (predators[j].canReproduce && predators[i].canReproduce)
                {
                    if (QRandomGenerator::global()->bounded(2) == 0)
                    {
						reproducePredators(i, j, newPredators, reproduced);
                        break;
                    }
                    else 
                    {
						FightPredators(i, j, dead);
                    }
                }
                else
                {
					FightPredators(i, j, dead);
                }
                if (dead[i]) break;
                if (reproduced[i]) break;
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

    for (const auto& newPredator : newPredators)
    {
        predators.push_back(newPredator);
    }
}

void Map::reproducePredators(int i, int j, std::vector<Predator>& newPredators, std::vector<bool>& reproduced)
{
    predators[i].reproduce(newPredators, N, N);
    predators[i].canReproduce = false;
    predators[j].canReproduce = false;
    predators[i].setLastReproduction(currentTime);
    predators[j].setLastReproduction(currentTime);
    reproduced[i] = true;
    reproduced[j] = true;
}

void Map::FightPredators(int i, int j, std::vector<bool>& dead)
{
    if (QRandomGenerator::global()->bounded(2) == 0)
    {
        dead[i] = true;
    }
    else
    {
        dead[j] = true;
    }
}

void Map::PredatorsEatPreys()
{
    std::vector<bool> dead(preys.size(), false);

    for (int i = 0; i < preys.size(); ++i)
    {
        if (dead[i]) continue;

        for (int j = 0; j < predators.size(); ++j)
        {
            if (predators[j].isHungry())
            {
                if (preys[i].getX() == predators[j].getX() && preys[i].getY() == predators[j].getY())
                {
                    dead[i] = true;
                    predators[j].resetSatiete();
                    break;
                }
            }
        }
    }

    std::vector<Prey> survivors;
    for (size_t i = 0; i < preys.size(); ++i)
    {
        if (!dead[i])
        {
            survivors.push_back(preys[i]);
        }
    }
    preys = std::move(survivors);
}

void Map::RemoveDeadPredators()
{
    std::vector<Predator> survivors;
    for (size_t i = 0; i < predators.size(); ++i)
    {
        if (!predators[i].isDeadStatus())
        {
            survivors.push_back(predators[i]);
        }
    }
    predators = std::move(survivors);
}
