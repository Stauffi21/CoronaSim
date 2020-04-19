#include "spielfigur.h"
#include "QDebug"

Spielfigur::Spielfigur(QPointF xy)
{
    Pos = xy;
    //asmxForce = randXForce;
    //myForce = 4 - fabs(randXForce);
    HitBox = new QRectF(0, 0, 20, 20);
    IsInfected = false;
}

QRectF Spielfigur::BoundingRect() const
{
    HitBox->moveCenter(QPointF(Pos.x(), Pos.y()));
    return *HitBox;
}

bool Spielfigur::isInfected() const
{
    return IsInfected;
}

void Spielfigur::infect()
{
    IsInfected = true;
}

void Spielfigur::removeInfect()
{
    IsInfected = false;
}
