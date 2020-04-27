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
    Spielfigur(QPointF xy, float speedXY, double sterben);

    QRectF BoundingRect();
    QPointF isPos();

    bool isInfected() const;
    void infect();
    void removeInfect();

    bool isActive() const;
    void active();
    void removeActive();

    void move();
    int isDirection() const;
    void changeSpeed(bool horizontal);
    void changeDirection(int direction);
    int isAlter() const;
    void setSterbensrate(double newValue);
    double Sterbensrate() const;
    bool isAlive();
    void setAlive();

signals:

public slots:

private:
    QRectF Rect;
    QPointF Pos;
    bool IsInfected;
    bool IsActive;
    int randomDirection;
    float speedX;
    float speedY;
    int randomAlter;
    double sterbensrate;
    bool alive;
    bool simulation;
};

#endif // SPIELFIGUR_H
