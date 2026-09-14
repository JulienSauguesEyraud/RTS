#include <QApplication>
#include <Map.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Map* map = new Map();
    map->show();
    map->start();

    return app.exec();
}
