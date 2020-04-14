#include "gameboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    gameboard gameboard;
    gameboard.show();
    return app.exec();
}
