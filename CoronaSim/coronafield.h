#ifndef CORONAFIELD_H
#define CORONAFIELD_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QList>
#include <QTimer>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QDateTime>
#include <QString>

#include "spielfigur.h"

class CoronaField : public QWidget
{
    Q_OBJECT
public:
    CoronaField(QWidget *parent = 0);
    void setValueMenschen(int newValue);
    void setValueInfizierte(int newValue);
    void setValueAktive(int newValue);
    int ValueMenschen() const;
    int ValueInfizierte() const;
    int ValueAktive() const;
    void resetSimulation(int resetMenschen, int resetInfizierte, int resetAktive);

protected:
    void paintEvent(QPaintEvent *);

private:
    int zufallsZahl(int low, int high);
    void createSpielfigur();
    void deleteSpielfigur();
    void paintSpielfiguren(QPainter &painter);

    QTimer *spielfigurTimer;
    QTimer *updateTimer;
    QTimer *updateTimerDaten;
    QList<Spielfigur *> spielfigurList;
    int valueMenschen;
    int valueInfizierte;
    int valueAktive;

signals:

public slots:
    void startSimulation();
    void stopSimulation();

private slots:
    void moveSpielfiguren();
};

#endif // CORONAFIELD_H
