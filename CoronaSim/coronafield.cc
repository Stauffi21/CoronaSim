#include "coronafield.h"
#include "gameboard.h"
#include "spielfigur.h"

#include <QDateTime>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QPointF>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QDateTime>

#include <math.h>
#include <stdlib.h>

CoronaField::CoronaField(QWidget *parent) : QWidget(parent)
{
    setPalette(QPalette(QColor(255,255,255)));
    setAutoFillBackground(true);
    valueMenschen=0;
    valueInfizierte=0;
    valueAktive = 0;
    spielfigurTimer = new QTimer(this);
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(moveSpielfiguren()));
    mRunning = false;
    mDiff = "0:0:0:0";
}

void CoronaField::setValueMenschen(int newValue){
    int differenz = valueMenschen-newValue;
    if(differenz>0){
        if(valueMenschen==valueInfizierte){
            valueInfizierte=valueInfizierte-differenz;
        }
        for(int i=differenz;i>0;i--){
        deleteSpielfigur();
        }
    }
    if(differenz<0){
        for(int i=differenz;i<0;i++){
            createSpielfigur();
        }
    }
    valueMenschen = newValue;

}

void CoronaField::setValueInfizierte(int newValue){
    if(valueMenschen==0||newValue>valueMenschen){
        newValue=valueMenschen;
    }
    int differenz = valueInfizierte-newValue;
    if(differenz>0){
        for(int i=valueInfizierte;i>newValue;i--){
        spielfigurList[i-1]->removeInfect();
        update();
        }
    }
    if(differenz<0){
        for(int i=valueInfizierte;i<newValue;i++){
            spielfigurList[i]->infect();
            update();
        }
    }
    valueInfizierte = newValue;
}

void CoronaField::setValueAktive(int newValue){
    int differenz = valueAktive-newValue;
    int anzahl = 0;
    if(differenz>0){
        anzahl = round(ValueMenschen()*0.01*differenz);
        for(int i=0;i<anzahl;i++){
        spielfigurList[i]->removeActive();
        qDebug()<<spielfigurList[i]->isActive();
        }
    }
    if(differenz<0){
        anzahl = round(ValueMenschen()*0.01*newValue);
        for(int i=0;i<anzahl;i++){
            spielfigurList[i]->active();
            qDebug()<<spielfigurList[i]->isActive();
        }
    }
    valueAktive = newValue;
}

int CoronaField::ValueMenschen() const{
    return valueMenschen;
}

int CoronaField::ValueInfizierte() const{
    return valueInfizierte;
}

int CoronaField::ValueAktive() const{
    return valueAktive;
}

int CoronaField::zufallsZahl(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

void CoronaField::createSpielfigur(){
    bool beruerung = false;
    int versuche = 0;
    QPointF startPosition;
    do {
            versuche += 1;
            beruerung = false;
            startPosition = QPointF(zufallsZahl(25, width() - 25), zufallsZahl(25, height() - 25));
            for(Spielfigur *spielfigur : spielfigurList) {
                if((startPosition - spielfigur->Pos).manhattanLength() < 40) {
                    beruerung = true;
                    break;
                }
            }
        } while(beruerung && versuche < 150);
        if(!beruerung) {
            Spielfigur *newSpielfigur = new Spielfigur(startPosition);
            spielfigurList.append(newSpielfigur);
            update();
        }
}

void CoronaField::deleteSpielfigur(){
    spielfigurList.removeLast();
    update();
}

void CoronaField::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    paintSpielfiguren(painter);
}

void CoronaField::timerEvent(QTimerEvent *)
{
    if(mRunning)
    {
        qint64 ms = mStartTime.msecsTo(QDateTime::currentDateTime());
        int h = ms / 1000 / 60 / 60;
        int m = (ms / 1000 / 60) - (h * 60);
        int s = (ms / 1000) - (m * 60);
        ms = ms - (s * 1000);
        mDiff = QString("%1:%2:%3:%4").arg(h).arg(m).arg(s).arg(ms);
    }
}

QString CoronaField::isStoppwatch() const{
    return mDiff;
}

void CoronaField::paintSpielfiguren(QPainter &painter)
{
    for(Spielfigur *spielfigur : spielfigurList) {
        painter.save();
        painter.setPen(Qt::NoPen);
        if(spielfigur->isInfected())
            painter.setBrush(Qt::red);
        else
            painter.setBrush(Qt::black);
        painter.drawEllipse(spielfigur->BoundingRect());
        painter.restore();
    }
}

void CoronaField::moveSpielfiguren(){
    for(Spielfigur *spielfigur : spielfigurList) {
            spielfigur->move();
            if(spielfigur->Pos.y() + 10 > height() || spielfigur->Pos.y() - 10 < 0) {
                int direction = spielfigur->isDirection();
                if(direction==1){
                    spielfigur->changeDirection(0);
                }
                else{
                    spielfigur->changeDirection(1);
                }
                continue;
            }
            if(spielfigur->Pos.x() + 10 > width() || spielfigur->Pos.x() - 10 < 0) {
                int direction = spielfigur->isDirection();
                if(direction==1){
                    spielfigur->changeDirection(0);
                }
                else{
                    spielfigur->changeDirection(1);
                }
            }
            for(Spielfigur *kollision : spielfigurList) {
                int direction = spielfigur->isDirection();
                if(spielfigur != kollision && (spielfigur->Pos - kollision->Pos).manhattanLength() < 20) {
                    if(direction==1){
                        spielfigur->changeDirection(0);
                    }
                    else{
                        spielfigur->changeDirection(1);
                    }
                    if(spielfigur->isInfected())
                        kollision->infect();
                }
            }
    }
}

void CoronaField::startSimulation(){
    mStartTime = QDateTime::currentDateTime();
    mRunning = true;
    if(ValueMenschen()==0){
        return;
    }
    else{
    spielfigurTimer->start(15);
    update();
    }
}

void CoronaField::stopSimulation(){
    mRunning = false;
    spielfigurTimer->stop();
}
