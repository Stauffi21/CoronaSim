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

#include "spielfigur.h"

class CoronaField : public QWidget
{
    Q_OBJECT
public:
    CoronaField(QWidget *parent = 0);
    void setValueMenschen(int newValue);
    void setValueInfizierte(int newValue);
    void setValueAktive(int newValue);
    void setValueSterben(double newValue);
    void setGesamtInfizierte(int anzahl);
    void setGesamtTote(int anzahl);
    int ValueMenschen() const;
    int ValueInfizierte() const;
    int ValueAktive() const;
    double ValueSterben() const;
    int GesamtInfizierte();
    int GesamtTote();
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
    double sterbensrate;
    int gesamtInfizierte;
    void Alive();
    int gesamtTote;
    QTimer *statusTimer;

signals:
    void infziert();
    void stop();
    void tot();

public slots:
    void startSimulation();
    void stopSimulation();

private slots:
    void moveSpielfiguren();
    void showInfizierte();
    void checkAlive();
    void showTote();
};

#endif // CORONAFIELD_H
