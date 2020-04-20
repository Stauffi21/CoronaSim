#ifndef SPIELFIGUR_H
#define SPIELFIGUR_H

#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QRectF>
#include <math.h>

class Spielfigur : public QObject
{
    Q_OBJECT
public:
    Spielfigur(QPointF xy);

    QRectF BoundingRect() const;
    QPointF Pos;

    bool isInfected() const;
    void infect();
    void removeInfect();

    bool isActive() const;
    void active();
    void removeActive();

    void move();
    int isDirection() const;
    void changeDirection(int direction);
signals:

public slots:

private:
    QRectF *Rect;
    bool IsInfected;
    bool IsActive;
    int randomDirection;
};

#endif // SPIELFIGUR_H
