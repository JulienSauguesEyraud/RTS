#include <Map.h>

#include <QPainter>
#include <QThread>
#include <fstream>

Map::Map(QWidget *parent):
    QWidget(parent)
{
    rabbitImage.load(":/images/rabbit.png");
    foxImage.load(":/images/fox.png");

    for (int i = 0; i < H0Rabbit; ++i)
    {
        rabbits.emplace_back(N, N);
    }
    for (int i = 0; i < H0Fox; ++i)
    {
        foxes.emplace_back(N, N);
    }
	for (int i = 0; i < H0Vegetation; ++i)
	{
        int nbTriesMax = 1000;
        int nbTries = 0;
        int x = QRandomGenerator::global()->bounded(N);
        int y = QRandomGenerator::global()->bounded(N);
        std::vector<int> occupiedXPositions;
        std::vector<int> occupiedYPositions;
        for (const auto& vegetation : vegetations)
        {
            occupiedXPositions.push_back(vegetation.getX());
            occupiedYPositions.push_back(vegetation.getY());
        }
        while (std::find(occupiedXPositions.begin(), occupiedXPositions.end(), x) != occupiedXPositions.end()
            && std::find(occupiedYPositions.begin(), occupiedYPositions.end(), y) != occupiedYPositions.end()
            && nbTries < nbTriesMax)
        {
            x = QRandomGenerator::global()->bounded(N);
            y = QRandomGenerator::global()->bounded(N);
            nbTries++;
        }
		if (nbTries < nbTriesMax)
		{
			vegetations.emplace_back(vegetations, N, N, x, y);
		}
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

	drawGrid(painter, cellWidth, cellHeight);
	drawVegetation(painter, cellWidth, cellHeight);
	drawRabbits(painter, cellWidth, cellHeight);
	drawFoxes(painter, cellWidth, cellHeight);

	painter.setFont(QFont("Arial", 12));
	painter.drawText(0, 12, QString("Lapins: " + QString::number(rabbits.size())));
    painter.drawText(0, 36, QString("Renards: " + QString::number(foxes.size())));
	painter.drawText(0, 54, QString("Jour: " + QString::number(currentTime)));
}

void Map::start()
{
    QThread *thread = QThread::create([this]()
    {
        std::ofstream file("evolution_populations.csv");
        file << "Temps;Lapins;Renards\n";

        while (true)
        {
            QThread::msleep(100);

            updateRabbits();
            updateFoxes();
			updateVegetation();
            removeDeadRabbits();
            removeDeadFoxes();
            foxesEatRabbits();
            checkFightOrReproduceFoxes();
			rabbitsEatVegetation();
            reproduceRabbits();

            currentTime += deltaT;

            file << currentTime << ";" << rabbits.size() << ";" << foxes.size() << "\n";
            file.flush();

            QMetaObject::invokeMethod(this, [this]() {
                update();
            });
        }
    });

    thread->start();
}

void Map::drawGrid(QPainter& painter, const int& cellWidth, const int& cellHeight) const
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

void Map::drawRabbits(QPainter& painter, const int& cellWidth, const int& cellHeight)
{
    for (const auto& rabbit : rabbits) {
        if (showImages && !rabbitImage.isNull())
        {
            if (rabbit.isChidren())
            {
                painter.drawImage(
                    QRect(rabbit.getX() * cellWidth + cellWidth / 3, rabbit.getY() * cellHeight + cellHeight / 4, cellWidth / 4, cellHeight / 2),
                    rabbitImage
                );
            }
            else
            {
                painter.drawImage(
                    QRect(rabbit.getX() * cellWidth + cellWidth / 4, rabbit.getY() * cellHeight, cellWidth / 2, cellHeight),
                    rabbitImage
                );
            }
        }
        else
        {
            if (rabbit.getIsMale())
            {
                if (rabbit.isChidren())
                {
                    painter.setBrush(Qt::blue);
                    painter.drawEllipse(rabbit.getX() * cellWidth + cellWidth / 4, rabbit.getY() * cellHeight + cellHeight / 4, cellWidth / 2, cellHeight / 2);
                }
                else if (!rabbit.canReproduce)
                {
                    painter.setBrush(Qt::darkBlue);
                    painter.drawEllipse(rabbit.getX() * cellWidth, rabbit.getY() * cellHeight, cellWidth, cellHeight);
                }
                else
                {
                    painter.setBrush(Qt::blue);
                    painter.drawEllipse(rabbit.getX() * cellWidth, rabbit.getY() * cellHeight, cellWidth, cellHeight);
                }
            }
            else
            {
                if (rabbit.isChidren())
                {
                    painter.setBrush(Qt::cyan);
                    painter.drawEllipse(rabbit.getX() * cellWidth + cellWidth / 4, rabbit.getY() * cellHeight + cellHeight / 4, cellWidth / 2, cellHeight / 2);
                }
                else if (!rabbit.canReproduce)
                {
                    painter.setBrush(Qt::darkCyan);
                    painter.drawEllipse(rabbit.getX() * cellWidth, rabbit.getY() * cellHeight, cellWidth, cellHeight);
                }
                else
                {
                    painter.setBrush(Qt::cyan);
                    painter.drawEllipse(rabbit.getX() * cellWidth, rabbit.getY() * cellHeight, cellWidth, cellHeight);
                }
            }
        }
    }
}

void Map::drawFoxes(QPainter& painter, const int& cellWidth, const int& cellHeight)
{
    for (const auto& fox : foxes) {
        if (showImages && !foxImage.isNull())
        {
            if (fox.isChidren())
            {
                painter.drawImage(
                    QRect(fox.getX() * cellWidth + cellWidth / 3, fox.getY() * cellHeight + cellHeight / 4, cellWidth / 3, cellHeight / 2),
                    foxImage
                );
            }
            else
            {
                painter.drawImage(
                    QRect(fox.getX() * cellWidth + cellWidth / 4, fox.getY() * cellHeight, cellWidth / 1.5, cellHeight),
                    foxImage
                );
            }
        }
        else
        {
            if (fox.getIsMale())
            {
                if (fox.isChidren() && !fox.isHungry())
                {
                    painter.setBrush(Qt::red);
                    painter.drawEllipse(fox.getX() * cellWidth + cellWidth / 4, fox.getY() * cellHeight + cellHeight / 4, cellWidth / 2, cellHeight / 2);
                }
                else if (fox.isChidren() && fox.isHungry())
                {
                    painter.setBrush(Qt::darkRed);
                    painter.drawEllipse(fox.getX() * cellWidth + cellWidth / 4, fox.getY() * cellHeight + cellHeight / 4, cellWidth / 2, cellHeight / 2);
                }
                else if (!fox.canReproduce && !fox.isHungry())
                {
                    painter.setBrush(Qt::yellow);
                    painter.drawEllipse(fox.getX() * cellWidth, fox.getY() * cellHeight, cellWidth, cellHeight);
                }
                else if (!fox.canReproduce && fox.isHungry())
                {
                    painter.setBrush(Qt::darkYellow);
                    painter.drawEllipse(fox.getX() * cellWidth, fox.getY() * cellHeight, cellWidth, cellHeight);
                }
                else if (fox.isHungry())
                {
                    painter.setBrush(Qt::darkRed);
                    painter.drawEllipse(fox.getX() * cellWidth, fox.getY() * cellHeight, cellWidth, cellHeight);
                }
                else
                {
                    painter.setBrush(Qt::red);
                    painter.drawEllipse(fox.getX() * cellWidth, fox.getY() * cellHeight, cellWidth, cellHeight);
                }
            }
            else
            {
                if (fox.isChidren() && !fox.isHungry())
                {
                    painter.setBrush(Qt::magenta);
                    painter.drawEllipse(fox.getX() * cellWidth + cellWidth / 4, fox.getY() * cellHeight + cellHeight / 4, cellWidth / 2, cellHeight / 2);
                }
                else if (fox.isChidren() && fox.isHungry())
                {
                    painter.setBrush(Qt::darkMagenta);
                    painter.drawEllipse(fox.getX() * cellWidth + cellWidth / 4, fox.getY() * cellHeight + cellHeight / 4, cellWidth / 2, cellHeight / 2);
                }
                else if (!fox.canReproduce && !fox.isHungry())
                {
                    painter.setBrush(Qt::gray);
                    painter.drawEllipse(fox.getX() * cellWidth, fox.getY() * cellHeight, cellWidth, cellHeight);
                }
                else if (!fox.canReproduce && fox.isHungry())
                {
                    painter.setBrush(Qt::darkGray);
                    painter.drawEllipse(fox.getX() * cellWidth, fox.getY() * cellHeight, cellWidth, cellHeight);
                }
                else if (fox.isHungry())
                {
                    painter.setBrush(Qt::darkMagenta);
                    painter.drawEllipse(fox.getX() * cellWidth, fox.getY() * cellHeight, cellWidth, cellHeight);
                }
                else
                {
                    painter.setBrush(Qt::magenta);
                    painter.drawEllipse(fox.getX() * cellWidth, fox.getY() * cellHeight, cellWidth, cellHeight);
                }
            }
        }
    }
}

void Map::drawVegetation(QPainter& painter, const int& cellWidth, const int& cellHeight)
{
    for (const auto& vegetation : vegetations)
    {
        painter.setBrush(Qt::darkGreen);
		int vegetationHeight = static_cast<int>(cellHeight * vegetation.getPercentageAmount());
		painter.drawRect(vegetation.getX() * cellWidth, vegetation.getY() * cellHeight + (cellHeight - vegetationHeight), cellWidth, vegetationHeight);
    }
}

void Map::updateRabbits()
{
    for (auto& rabbit : rabbits) {
        rabbit.update(N, N, deltaT, currentTime);
    }
}

void Map::updateFoxes()
{
    for (auto& fox : foxes) {
        fox.update(N, N, deltaT, currentTime);
    }
}

void Map::updateVegetation()
{
	for (auto& vegetation : vegetations) {
		vegetation.update(vegetations, N, N);
	}
}

void Map::reproduceRabbits()
{
	std::vector<Rabbit> newRabbits;
	for (int i = 0; i < rabbits.size(); ++i)
	{
		if (!rabbits[i].canReproduce) continue;

		for (int j = i + 1; j < rabbits.size(); ++j)
		{
            if (rabbits[j].canReproduce)
            {
                if (rabbits[i].getIsMale() != rabbits[j].getIsMale())
				{
                    if (rabbits[i].getX() == rabbits[j].getX() && rabbits[i].getY() == rabbits[j].getY())
                    {
                        rabbits[i].reproduce(newRabbits, N, N);
                        rabbits[i].canReproduce = false;
                        rabbits[j].canReproduce = false;
                        rabbits[i].setLastReproduction(currentTime);
                        rabbits[j].setLastReproduction(currentTime);
                        break;
                    }
                }
			}
		}
	}

	for (const auto& newRabbit : newRabbits)
	{
		rabbits.push_back(newRabbit);
	}
}

void Map::rabbitsEatVegetation()
{
	for (auto& rabbit : rabbits)
	{
		for (auto& vegetation : vegetations)
		{
			if (rabbit.getX() == vegetation.getX() && rabbit.getY() == vegetation.getY())
			{
				if (vegetation.canBeEaten(rabbit.getAmoutVegetationToEat()))
				{
					vegetation.reduceVegetation(rabbit.getAmoutVegetationToEat());
					rabbit.resetSatiete();
				}
			}
		}
	}
}

void Map::checkFightOrReproduceFoxes()
{
    std::vector<Fox> newFoxes;
    std::vector<bool> dead(foxes.size(), false);
    std::vector<bool> reproduced(foxes.size(), false);

    for (int i = 0; i < foxes.size(); ++i)
    {
        if (dead[i]) continue;
        if (reproduced[i]) continue;
        if (foxes[i].isChidren()) continue;

        for (int j = i + 1; j < foxes.size(); ++j)
        {
            if (dead[j]) continue;
            if (reproduced[j]) continue;
            if (foxes[j].isChidren()) continue;

            if (foxes[i].getX() == foxes[j].getX() && foxes[i].getY() == foxes[j].getY())
            {
                if (foxes[i].getIsMale() != foxes[j].getIsMale())
                {
                    if (foxes[j].canReproduce && foxes[i].canReproduce)
                    {
                        if (QRandomGenerator::global()->bounded((foxes[i].getSatiete() + foxes[j].getSatiete()) / 2 + std::max(foxes[i].getSatiete(), foxes[j].getSatiete())) == 0)
                        {
                            fightFoxes(i, j, dead);
                        }
                        else
                        {
                            reproduceFoxes(i, j, newFoxes, reproduced);
                            break;
                        }
                    }
                    else
                    {
                        fightFoxes(i, j, dead);
                    }
                }
                else
                {
                    fightFoxes(i, j, dead);
                }
                if (dead[i]) break;
                if (reproduced[i]) break;
            }
        }
    }

    std::vector<Fox> survivors;
    for (size_t i = 0; i < foxes.size(); ++i)
    {
        if (!dead[i])
        {
            survivors.push_back(foxes[i]);
        }
    }
    foxes = std::move(survivors);

    for (const auto& newFox : newFoxes)
    {
        foxes.push_back(newFox);
    }
}

void Map::reproduceFoxes(int i, int j, std::vector<Fox>& newFoxes, std::vector<bool>& reproduced)
{
    foxes[i].reproduce(newFoxes, N, N);
    foxes[i].canReproduce = false;
    foxes[j].canReproduce = false;
    foxes[i].setLastReproduction(currentTime);
    foxes[j].setLastReproduction(currentTime);
    reproduced[i] = true;
    reproduced[j] = true;
}

void Map::fightFoxes(int i, int j, std::vector<bool>& dead)
{
    // les prédateurs mangent le vaincu uniquement quand ils sont très affamés, sinon ils se battent juste et le vaincu meurt
    if (!foxes[i].isHungry() && !foxes[j].isHungry())
    {
        if (foxes[i].getIsMale() == foxes[j].getIsMale())
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
		else if (foxes[i].getIsMale() && !foxes[j].getIsMale())
		{
			if (QRandomGenerator::global()->bounded(3) == 0) // prédateur mâle a 66% de chances de tuer le prédateur femelle
			{
				dead[j] = true;
			}
			else
			{
				dead[i] = true;
			}
		}
		else
		{
			if (QRandomGenerator::global()->bounded(3) == 0) // prédateur femelle a 33% de chances de tuer le prédateur mâle
			{
				dead[i] = true;
			}
			else
			{
				dead[j] = true;
			}
		}
    }

    else if (foxes[i].isHungry() && !foxes[j].isHungry())
    {
		if (foxes[i].getIsMale() == foxes[j].getIsMale())
		{
            if (foxes[i].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(5) == 0) // prédateur très affamé a 20% de chances de tuer le prédateur pas affamé
                {
                    dead[j] = true;
                    foxes[i].resetSatiete();
                }
                else
                {
                    dead[i] = true;
                }
            }
            else
            {
                if (QRandomGenerator::global()->bounded(5) == 0) // prédateur un peu affamé a 80% de chances de tuer le prédateur pas affamé
                {
                    dead[i] = true;
                }
                else
                {
                    dead[j] = true;
                }
            }
		}
        else if (foxes[i].getIsMale() && !foxes[j].getIsMale())
        {
            if (foxes[i].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(3) == 0) // mâle très affamé a 33% de chances de tuer la femelle pas affamé
                {
                    dead[j] = true;
                    foxes[i].resetSatiete();
                }
                else
                {
                    dead[i] = true;
                }
            }
            else
            {
                if (QRandomGenerator::global()->bounded(10) == 0) // mâle un peu affamé a 90% de chances de tuer la femelle pas affamé
                {
                    dead[i] = true;
                }
                else
                {
                    dead[j] = true;
                }
            }
        }
        else
        {
            if (foxes[i].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(10) == 0) // femelle très affamé a 10% de chances de tuer le mâle pas affamé
                {
                    dead[j] = true;
                    foxes[i].resetSatiete();
                }
                else
                {
                    dead[i] = true;
                }
            }
            else
            {
                if (QRandomGenerator::global()->bounded(3) == 0) // femelle un peu affamé a 66% de chances de tuer le mâle pas affamé
                {
                    dead[i] = true;
                }
                else
                {
                    dead[j] = true;
                }
            }
        }
    }

    else if (!foxes[i].isHungry() && foxes[j].isHungry())
    {
        if (foxes[i].getIsMale() == foxes[j].getIsMale())
        {
            if (foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(5) == 0) // prédateur très affamé a 20% de chances de tuer le prédateur pas affamé
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                }
                else
                {
                    dead[j] = true;
                }
            }
            else
            {
                if (QRandomGenerator::global()->bounded(5) == 0) // prédateur un peu affamé a 80% de chances de tuer le prédateur pas affamé
                {
                    dead[j] = true;
                }
                else
                {
                    dead[i] = true;
                }
            }
        }
        else if (foxes[i].getIsMale() && !foxes[j].getIsMale())
        {
            if (foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(10) == 0) // femelle très affamé a 10% de chances de tuer le mâle pas affamé
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                }
                else
                {
                    dead[j] = true;
                }
            }
            else
            {
                if (QRandomGenerator::global()->bounded(3) == 0) // femelle un peu affamé a 66% de chances de tuer le mâle pas affamé
                {
                    dead[j] = true;
                }
                else
                {
                    dead[i] = true;
                }
            }
        }
        else
        {
            if (foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(3) == 0) // mâle très affamé a 33% de chances de tuer la femelle pas affamé
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                }
                else
                {
                    dead[j] = true;
                }
            }
            else
            {
                if (QRandomGenerator::global()->bounded(10) == 0) // mâle un peu affamé a 90% de chances de tuer la femelle pas affamé
                {
                    dead[j] = true;
                }
                else
                {
                    dead[i] = true;
                }
            }
        }
    }

    else if (foxes[i].isHungry() && foxes[j].isHungry())
    {
        if (foxes[i].getIsMale() == foxes[j].getIsMale())
        {
            if (!foxes[i].isVeryHungry() && foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(10) == 0) // prédateur très affamé a 10% de chances de tuer le prédateur un peu affamé
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                }
                else
                {
                    dead[j] = true;
                }
            }
            else if (foxes[i].isVeryHungry() && !foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(10) == 0) // prédateur un peu affamé a 90% de chances de tuer le prédateur très affamé
                {
                    dead[j] = true;
                    foxes[i].resetSatiete();
                }
                else
                {
                    dead[i] = true;
                }
            }
            else if (foxes[i].isVeryHungry() && foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(2) == 0) // les 2 prédateurs très affamés ont 50% de chance de tuer l'autre
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                }
                else
                {
                    dead[j] = true;
                    foxes[i].resetSatiete();
                }
            }
            else
            {
                if (QRandomGenerator::global()->bounded(2) == 0) // les 2 prédateurs un peu affamés ont 50% de chance de tuer l'autre
                {
                    dead[i] = true;
                }
                else
                {
                    dead[j] = true;
                }
            }
        }
        else if (foxes[i].getIsMale() && !foxes[j].getIsMale())
        {
            if (!foxes[i].isVeryHungry() && foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(20) == 0) // mâle un peu affamé a 95% de chances de tuer la femelle très affamé
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                }
                else
                {
                    dead[j] = true;
                }
            }
            else if (foxes[i].isVeryHungry() && !foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(4) == 0) // femelle un peu affamé a 75% de chances de tuer le mâle très affamé
                {
                    dead[j] = true;
                    foxes[i].resetSatiete();
                }
                else
                {
                    dead[i] = true;
                }
            }
            else if (foxes[i].isVeryHungry() && foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(3) == 0) // le mâle très affamé a 66% de chance de tuer la femelle très affamé
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                }
                else
                {
                    dead[j] = true;
                    foxes[i].resetSatiete();
                }
            }
            else
            {
				if (QRandomGenerator::global()->bounded(3) == 0) // le mâle un peu affamé a 66% de chance de tuer la femelle un peu affamé
                {
                    dead[i] = true;
                }
                else
                {
                    dead[j] = true;
                }
            }
        }
        else
        {
            if (!foxes[i].isVeryHungry() && foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(4) == 0) // femelle un peu affamé a 75% de chances de tuer le mâle très affamé
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                }
                else
                {
                    dead[j] = true;
                }
            }
            else if (foxes[i].isVeryHungry() && !foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(20) == 0) // mâle un peu affamé a 95% de chances de tuer la femelle très affamé
                {
                    dead[j] = true;
                    foxes[i].resetSatiete();
                }
                else
                {
                    dead[i] = true;
                }
            }
            else if (foxes[i].isVeryHungry() && foxes[j].isVeryHungry())
            {
                if (QRandomGenerator::global()->bounded(3) == 0) // mâle très affamé a 66% de chance de tuer la femelle très affamé
                {
                    dead[j] = true;
                    foxes[i].resetSatiete();
                }
                else
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                }
            }
            else
            {
                if (QRandomGenerator::global()->bounded(3) == 0) // mâle un peu affamé a 66% de chance de tuer la femelle un peu affamée
                {
                    dead[j] = true;
                }
                else
                {
                    dead[i] = true;
                }
            }
        }
    }
}

void Map::foxesEatRabbits()
{
    std::vector<bool> dead(rabbits.size(), false);

    for (int i = 0; i < rabbits.size(); ++i)
    {
        if (dead[i]) continue;

        for (int j = 0; j < foxes.size(); ++j)
        {
            if (foxes[j].isHungry())
            {
                if (rabbits[i].getX() == foxes[j].getX() && rabbits[i].getY() == foxes[j].getY())
                {
                    dead[i] = true;
                    foxes[j].resetSatiete();
                    break;
                }
            }
        }
    }

    std::vector<Rabbit> survivors;
    for (size_t i = 0; i < rabbits.size(); ++i)
    {
        if (!dead[i])
        {
            survivors.push_back(rabbits[i]);
        }
    }
    rabbits = std::move(survivors);
}

void Map::removeDeadFoxes()
{
    std::vector<Fox> survivors;
    for (size_t i = 0; i < foxes.size(); ++i)
    {
        if (!foxes[i].isDeadStatus())
        {
            survivors.push_back(foxes[i]);
        }
    }
    foxes = std::move(survivors);
}

void Map::removeDeadRabbits()
{
    std::vector<Rabbit> survivors;
    for (size_t i = 0; i < rabbits.size(); ++i)
    {
        if (!rabbits[i].isDeadStatus())
        {
            survivors.push_back(rabbits[i]);
        }
    }
    rabbits = std::move(survivors);
}
