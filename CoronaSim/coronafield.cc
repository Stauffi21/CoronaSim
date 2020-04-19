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
    spielfigurTimer->setInterval(15);
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
    if(valueMenschen==0||newValue>=valueMenschen){
        valueInfizierte=valueMenschen;
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
    valueAktive = newValue;
}

int CoronaField::ValueMenschen(){
    return valueMenschen;
}

int CoronaField::ValueInfizierte(){
    return valueInfizierte;
}

int CoronaField::ValueAktive(){
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
    //float x = float(zufallsZahl(-20,20)) / 10;
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
        //spielfigur->paintSpielfigur(painter);
        if(spielfigur->isInfected())
            painter.setBrush(Qt::red);
        else
            painter.setBrush(Qt::black);
        painter.drawEllipse(spielfigur->BoundingRect());
        painter.restore();
    }
}
