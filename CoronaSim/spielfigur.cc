#include "spielfigur.h"
#include "QDebug"

Spielfigur::Spielfigur(QPointF xy)
{
    Pos = xy;
    //asmxForce = randXForce;
    //myForce = 4 - fabs(randXForce);
    HitBox = new QRectF(0, 0, 20, 20);
    IsInfected = false;
    IsMovable = false;
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

bool Spielfigur::isMovable() const
{
    return IsMovable;
}

void Spielfigur::infect()
{
    IsInfected = true;
}

void Spielfigur::movable(){
    IsMovable = true;
}

void Spielfigur::removeInfect()
{
    IsInfected = false;
}

void Spielfigur::removeMovable(){
    IsMovable = false;
}

void Spielfigur::move(){
    if(IsMovable){
        Pos.setX(Pos.x()+1);
        Pos.setY(Pos.y()+1);
    }
    else{
        return;
    }
}
