#include "spielfigur.h"
#include "QDebug"
#include <math.h>
#include <QElapsedTimer>

Spielfigur::Spielfigur(QPointF xy,float speedXY,double sterben)
{
    Pos = xy;
    Rect = QRectF(0, 0, 20, 20);
    IsInfected = false;
    IsActive = false;
    randomDirection = qrand()%5;
    speedX = speedXY;
    speedY = speedXY;
    randomAlter = (qrand()% ((99+0)-0)) + 0;
    this->setSterbensrate(sterben);
    alive = true;
}

QRectF Spielfigur::BoundingRect()
{
    Rect.moveCenter(QPointF(Pos.x(), Pos.y()));
    return Rect;
}

QPointF Spielfigur::isPos(){
    return Pos;
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
    float alter = randomAlter/100;
    if(!isActive()){
        return;
    }
    if(randomDirection == 0){
        Pos.setX(Pos.x()+(speedX-alter));
        Pos.setY(Pos.y()+(speedY-alter));
    }
    if(randomDirection == 1){
        Pos.setX(Pos.x()-(speedX-alter));
        Pos.setY(Pos.y()-(speedY-alter));
    }
    if(randomDirection == 2){
        Pos.setX(Pos.x()+(speedX-alter));
    }
    if(randomDirection == 3){
        Pos.setX(Pos.x()-(speedX-alter));
    }
    if(randomDirection == 4){
        Pos.setY(Pos.y()+(speedY-alter));
    }
    if(randomDirection == 5){
        Pos.setY(Pos.y()-(speedY-alter));
    }
}

void Spielfigur::moveBack(){
    float alter = randomAlter/100;
    if(!isActive()){
        return;
    }
    if(randomDirection == 0){
        Pos.setX(Pos.x()-(speedX-alter));
        Pos.setY(Pos.y()-(speedY-alter));
    }
    if(randomDirection == 1){
        Pos.setX(Pos.x()+(speedX-alter));
        Pos.setY(Pos.y()+(speedY-alter));
    }
    if(randomDirection == 2){
        Pos.setX(Pos.x()-(speedX-alter));
    }
    if(randomDirection == 3){
        Pos.setX(Pos.x()+(speedX-alter));
    }
    if(randomDirection == 4){
        Pos.setY(Pos.y()-(speedY-alter));
    }
    if(randomDirection == 5){
        Pos.setY(Pos.y()+(speedY-alter));
    }
}

void Spielfigur::changeSpeed(bool horizontal){
    if(horizontal){
        speedX *= -1;
    }
    else{
        speedY *= -1;
    }
}

void Spielfigur::changeDirection(int direction){
    randomDirection = direction;
}

int Spielfigur::isDirection() const{
    return randomDirection;
}

int Spielfigur::isAlter() const{
    return randomAlter;
}

void Spielfigur::setSterbensrate(double newValue){

    if(isAlter()<40){
        sterbensrate = newValue/16;
    }
    else if(isAlter()<50){
        sterbensrate = newValue/8.0;
    }
    else if(isAlter()<60){
        sterbensrate = newValue/2.46;
    }
    else if(isAlter()<70){
        sterbensrate = newValue/0.89;
    }
    else if(isAlter()<80){
        sterbensrate = newValue/0.4;
    }
    else if(isAlter()<100){
        sterbensrate = newValue/0.22;
    }
    if(sterbensrate>=100){
        sterbensrate = 100;
    }
}

double Spielfigur::Sterbensrate() const{
    return sterbensrate;
}

void Spielfigur::setAlive(){
    double wahrscheinlichkeit = 100-sterbensrate;
    double val = ((double)(rand()%100)/100.0);
    int random = 0;
    if(val < (wahrscheinlichkeit*0.01)){
        random = 0;
    }
    else if(val<(sterbensrate*0.01)){
        random = 1;
    }

    if(random == 0){
        return;
    }
    else if(random ==1){
        this->removeInfect();
        alive = false;
    }
}

bool Spielfigur::isAlive(){
    return alive;
}

void Spielfigur::Alive(){
    alive = true;
}
