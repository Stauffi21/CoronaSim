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
#include <QString>

#include <math.h>
#include <stdlib.h>

CoronaField::CoronaField(QWidget *parent) : QWidget(parent)
{
    updateTimer = new QTimer(this);
    updateTimer->setInterval(1000/17);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));

    updateTimerDaten = new QTimer(this);
    updateTimer->setInterval(10);

    setMinimumSize(600, 400);
    setPalette(QPalette(QColor(255,255,255)));
    setAutoFillBackground(true);

    valueMenschen=0;
    valueInfizierte=0;
    valueAktive = 0;
    sterbensrate = 0.0;
    gesamtInfizierte = 0;

    spielfigurTimer = new QTimer(this);
    spielfigurTimer->setInterval(15);
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(moveSpielfiguren()));
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(showInfizierte()));
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
    if(differenz==0){
        for(int i=0;i<newValue;i++){
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
    if(differenz==0){
        for(int i=0;i<newValue;i++){
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
        for(Spielfigur *spielfigur : spielfigurList) {
            spielfigur->removeActive();
            //qDebug()<<spielfigur->isActive();
        }
    }
    anzahl = round(ValueMenschen()*0.01*newValue);
    for(int i=0;i<anzahl;i++){
        spielfigurList[i]->active();
        //qDebug()<<spielfigurList[i]->isActive();
    }
    valueAktive = newValue;
}

void CoronaField::setValueSterben(double newValue){
    newValue = sterbensrate;
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

double CoronaField::ValueSterben()const {
    return sterbensrate;
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
            if((startPosition - spielfigur->isPos()).manhattanLength() < 40) {
                beruerung = true;
                break;
            }
        }
    } while(beruerung && versuche < 150);
    if(!beruerung) {
        Spielfigur *newSpielfigur = new Spielfigur(startPosition,1);
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
    if(GesamtInfizierte()==ValueMenschen()){
        painter.setPen(Qt::black);
        painter.setFont(QFont("Courier", 48, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, tr("Alle Infiziert"));
    }
    else{
        for(Spielfigur *spielfigur : spielfigurList) {
            painter.save();
            painter.setPen(Qt::NoPen);
            if(spielfigur->isInfected())
                painter.setBrush(Qt::red);
            else
                painter.setBrush(Qt::black);
            painter.drawEllipse(spielfigur->BoundingRect());
            painter.restore();
        }}
}

void CoronaField::moveSpielfiguren(){
    for(Spielfigur *spielfigur : spielfigurList) {
        spielfigur->move();
        if(spielfigur->isPos().y() + 12.5 > height() || spielfigur->isPos().y() - 12.5 < 0) {
            spielfigur->changeSpeed(false);
            continue;
        }
        if(spielfigur->isPos().x() + 12.5 > width() || spielfigur->isPos().x() - 12.5 < 0) {
            spielfigur->changeSpeed(true);
        }
        for(Spielfigur *kollision : spielfigurList) {
            if(spielfigur != kollision && (spielfigur->isPos() - kollision->isPos()).manhattanLength() < 22.5) {
                spielfigur->changeDirection(qrand()%5);
                if(spielfigur->isInfected())
                    kollision->infect();
            }
        }
    }
}

void CoronaField::startSimulation(){
    updateTimer->start();
    if(ValueMenschen()==0){
        return;
    }
    else{
        spielfigurTimer->start();
    }
}

void CoronaField::stopSimulation(){
    spielfigurTimer->stop();
    updateTimer->stop();
    update();
}

void CoronaField::resetSimulation(int resetMenschen, int resetInfizierte, int resetAktive){
    spielfigurTimer->stop();
    updateTimer->stop();
    valueMenschen = resetMenschen;
    valueInfizierte = resetInfizierte;
    valueAktive = resetAktive;
    qDeleteAll(spielfigurList);
    spielfigurList.clear();
    this->setValueMenschen(valueMenschen);
    this->setValueInfizierte(valueInfizierte);
    this->setGesamtInfizierte(valueInfizierte);
    this->setValueAktive(valueAktive);
}

void CoronaField::setGesamtInfizierte(int anzahl){
    gesamtInfizierte = anzahl;
    emit infziert();
}

int CoronaField::GesamtInfizierte(){
    return gesamtInfizierte;
}

void CoronaField::showInfizierte(){
    int anzahl = 0;
    for(Spielfigur *spielfigur : spielfigurList) {
        if(spielfigur->isInfected()){
            anzahl = anzahl+1;
            this->setGesamtInfizierte(anzahl);
        }
    }
    if(anzahl==ValueMenschen()){
        stopSimulation();
        emit stop();
    }
}
