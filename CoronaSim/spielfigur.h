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
signals:

public slots:

private:
    QRectF *HitBox;
    bool IsInfected;
};

#endif // SPIELFIGUR_H
