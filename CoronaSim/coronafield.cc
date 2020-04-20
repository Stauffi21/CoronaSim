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

#include <math.h>
#include <stdlib.h>

CoronaField::CoronaField(QWidget *parent) : QWidget(parent)
{
    setPalette(QPalette(QColor(255,255,255)));
    setAutoFillBackground(true);
    valueMenschen=0;
    valueInfizierte=0;
    valueAktive = 0;
    QTimer *spielfigurTimer = new QTimer();
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(moveSpielfiguren()));
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
    int anzahl = round(ValueMenschen()*0.01*newValue);
    qDebug() << anzahl;
    int differenz = valueAktive-newValue;
    if(differenz>0){
        for(int i=0;i<anzahl;i++){
        spielfigurList[i]->removeMovable();
        }
    }
    if(differenz<0){
        for(int i=0;i<anzahl;i++){
            spielfigurList[i]->movable();
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
    }
}

void CoronaField::startSimulation(){
    if(ValueMenschen()==0){
        return;
    }
    //spielfigurTimer->setInterval(15);
    spielfigurTimer->start(15);
}

void CoronaField::stopSimulation(){
    spielfigurTimer->stop();
}
