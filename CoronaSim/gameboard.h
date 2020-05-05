#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>
#include <QString>

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
    QTimer *recordTimer;

private slots:
    void anzMenschen(int newValue);
    void anzInfizierte(int newValue);
    void anzAktive(int newValue);
    void anzSterben(double newValue);
    void startSimulation();
    void stopSimulation();
    void resetSimulation();
    void showTime();
    void showInfizierte();
    void showTote();
    void showImmune();
    void aufzeichnenSimulation(int aufzeichnen);
    void isExport();
    void saveData();
};

#endif // GAMEBOARD_H
