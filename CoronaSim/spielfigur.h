#ifndef SPIELFIGUR_H
#define SPIELFIGUR_H

#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QRectF>
#include <math.h>
#include <QTimer>

class Spielfigur : public QObject
{
    Q_OBJECT
public:
    Spielfigur(QPointF xy, float speedXY);

    QRectF BoundingRect();
    QPointF isPos();

    bool isInfected() const;
    void infect();
    void removeInfect();

    bool isActive() const;
    void active();
    void removeActive();

    void move();
    void moveBack();
    int isDirection() const;
    void changeSpeed(bool horizontal);
    void changeDirection(int direction);
    int isAlter() const;
    void setToDie(bool newValue);
    bool toDie() const;
    bool isAlive();
    void Alive();
    void setToRecover(bool newValue);
    bool toRecover();
    bool isImmune();
    bool isIncubation();
    void simulationStopped();
    void simulationStarted();


signals:

public slots:

private slots:
    void setAlive();
    void setImmune();
    void setIncubation();

private:
    QRectF Rect;
    QPointF Pos;
    bool IsInfected;
    bool IsActive;
    int randomDirection;
    float speedX;
    float speedY;
    int randomAlter;
    bool alive;
    bool simulation;
    bool die;
    QTimer *toDieTimer;
    QTimer *immunityTimer;
    QTimer *incubationTimer;
    bool immune;
    bool recover;
    bool incubation;
    int stopTimeDie;
    int stopTimeImmunity;
    int stopTimeIncubation;
};

#endif // SPIELFIGUR_H
