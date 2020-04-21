#include "spielfigur.h"
#include "QDebug"
#include <math.h>

Spielfigur::Spielfigur(QPointF xy,float speedXY)
{
    Pos = xy;
    Rect = new QRectF(0, 0, 20, 20);
    IsInfected = false;
    IsActive = false;
    randomDirection = qrand()%2;
    speedX = speedXY;
    speedY = speedXY;
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
        Pos.setX(Pos.x()+speedX);
        Pos.setY(Pos.y()+speedY);
        qDebug()<<BoundingRect();
        qDebug()<<randomDirection;
    }
    if(randomDirection == 1){
        Pos.setX(Pos.x()-speedX);
        Pos.setY(Pos.y()-speedY);
        qDebug()<<BoundingRect();
        qDebug()<<randomDirection;
    }
}

void Spielfigur::changeDirection(bool horizontal){
    if(horizontal){
        speedX *= -1;
    }
    else{
        speedY *= -1;
    }

}

int Spielfigur::isDirection() const{
    return randomDirection;
}
