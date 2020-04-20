#ifndef SPIELFIGUR_H
#define SPIELFIGUR_H

#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QRectF>

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
    bool isMovable() const;
    void movable();
    void removeMovable();
    void move();
signals:

public slots:

private:
    QRectF *HitBox;
    bool IsInfected;
    bool IsMovable;
};

#endif // SPIELFIGUR_H
