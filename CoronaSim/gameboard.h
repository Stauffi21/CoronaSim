#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>

class CoronaField;

namespace Ui {
class gameboard;
}

class gameboard : public QWidget
{
    Q_OBJECT

public:
    explicit gameboard(QWidget *parent = 0);
    ~gameboard();

private:
    Ui::gameboard *ui;
    CoronaField *pCoronaField;

private slots:
    void reset();
    void anzMenschen(int i);
    void anzInfizierte(int i);
    void anzAktive(int i);
    void startSimulation();
    void stopSimulation();
};

#endif // GAMEBOARD_H
