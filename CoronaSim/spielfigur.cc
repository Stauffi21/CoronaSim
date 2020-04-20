#include "spielfigur.h"
#include "QDebug"
#include <math.h>

Spielfigur::Spielfigur(QPointF xy)
{
    Pos = xy;
    Rect = new QRectF(0, 0, 20, 20);
    IsInfected = false;
    IsActive = false;
    randomDirection = qrand()%2;
}

QRectF Spielfigur::BoundingRect() const
{
    Rect->moveCenter(QPointF(Pos.x(), Pos.y()));
    return *Rect;
}

bool Spielfigur::isInfected() const
{
    return IsInfected;
}

bool Spielfigur::isActive() const
{
    return IsActive;
}

void Spielfigur::infect()
{
    IsInfected = true;
}

void Spielfigur::active(){
    IsActive = true;
}

void Spielfigur::removeInfect()
{
    IsInfected = false;
}

void Spielfigur::removeActive(){
    IsActive = false;
}

void Spielfigur::move(){
    if(!isActive()){
        return;
    }
    if(randomDirection == 0){
        Pos.setX(Pos.x()+1);
        Pos.setY(Pos.y()+1);
        qDebug()<<BoundingRect();
        qDebug()<<randomDirection;
    }
    if(randomDirection == 1){
        Pos.setX(Pos.x()-1);
        Pos.setY(Pos.y()-1);
        qDebug()<<BoundingRect();
        qDebug()<<randomDirection;
    }
}

int Spielfigur::isDirection() const{
    return randomDirection;
}

void Spielfigur::changeDirection(int direction){
    randomDirection = direction;
}
