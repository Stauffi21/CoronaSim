#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>

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
    QTime *time;
    QElapsedTimer *elapsedTimer;
    int simulationStopped;
    QTimer *timer;

private slots:
    void anzMenschen(int i);
    void anzInfizierte(int i);
    void anzAktive(int i);
    void startSimulation();
    void stopSimulation();
    void resetSimulation();
    void showTime();
};

#endif // GAMEBOARD_H
