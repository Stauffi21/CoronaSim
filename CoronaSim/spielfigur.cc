#include "spielfigur.h"
#include "QDebug"
#include <math.h>
#include <QTimer>

Spielfigur::Spielfigur(QPointF xy,float speedXY)
{
    Pos = xy;
    Rect = QRectF(0, 0, 20, 20);
    IsInfected = false;
    IsActive = false;
    randomDirection = qrand()%5;
    speedX = speedXY;
    speedY = speedXY;
    randomAlter = (qrand()% ((99+0)-0)) + 0;
    alive = true;
    die = false;
    immune = false;
    recover = false;
    incubation = true;
    stopTimeDie = 0;
    stopTimeImmunity = 0;
    stopTimeIncubation = 0;
    toDieTimer = new QTimer(this);
    connect(toDieTimer,SIGNAL(timeout()),this,SLOT(setAlive()));
    toDieTimer->setSingleShot(true);

    immunityTimer = new QTimer(this);
    connect(immunityTimer,SIGNAL(timeout()),this,SLOT(setImmune()));
    immunityTimer->setSingleShot(true);

    incubationTimer = new QTimer(this);
    connect(incubationTimer,SIGNAL(timeout()),this,SLOT(setIncubation()));
    incubationTimer->setSingleShot(true);
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

void Spielfigur::setToDie(bool newValue){
    die = newValue;
    immunityTimer->stop();
    toDieTimer->start(8000);
}

bool Spielfigur::toDie() const{
    return die;
}

void Spielfigur::setAlive(){
    removeInfect();
    die = false;
    alive=false;
    incubation = false;
    recover = false;
}

bool Spielfigur::isAlive(){
    return alive;
}

void Spielfigur::Alive(){
    alive = true;
}

void Spielfigur::setToRecover(bool newValue){
    recover=newValue;
    incubationTimer->start(5000);
    immunityTimer->start(14000);
}

bool Spielfigur::toRecover(){
    return recover;
}

void Spielfigur::setImmune(){
    removeInfect();
    incubationTimer->stop();
    setToRecover(false);
    immune=true;
}

bool Spielfigur::isImmune(){
    return immune;
}

void Spielfigur::setIncubation(){
    incubation = false;
}

bool Spielfigur::isIncubation(){
    return incubation;
}

void Spielfigur::simulationStopped(){
    if(toDieTimer->isActive()){
        stopTimeDie=toDieTimer->remainingTime();
        toDieTimer->stop();
    }
    if(immunityTimer->isActive()){
        stopTimeImmunity = immunityTimer->remainingTime();
        immunityTimer->stop();
    }
    if(incubationTimer->isActive()){
        stopTimeIncubation = incubationTimer->remainingTime();
        incubationTimer->stop();
    }
}

void Spielfigur::simulationStarted(){
    if(toDie()){
        toDieTimer->start(stopTimeDie);
    }
    if(toRecover()){
        immunityTimer->start(stopTimeImmunity);
    }
    if(isIncubation()){
        incubationTimer->start(stopTimeIncubation);
    }
}
