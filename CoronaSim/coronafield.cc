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
#include <QFile>
#include <QFileDialog>

#include <math.h>
#include <stdlib.h>

CoronaField::CoronaField(QWidget *parent) : QWidget(parent)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    updateTimer = new QTimer(this);
    updateTimer->setInterval(1000/17);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));

    updateTimerDaten = new QTimer(this);
    updateTimer->setInterval(10);

    setMinimumSize(600, 400);
    setPalette(QPalette(QColor(255,255,255)));
    setAutoFillBackground(true);

    if(file.exists("Simulation.csv")){
        file.remove("Simulation.csv");
    }

    valueMenschen=0;
    valueInfizierte=0;
    valueAktive = 0;
    sterbensrate = 0;
    gesamtInfizierte = 0;
    gesamtTote = 0;
    gesamtImmune = 0;
    timeStopped = 0;
    aufzeichnen = false;
    aufzeichnungsnummer = 1;
    spielfigurTimer = new QTimer(this);
    spielfigurTimer->setInterval(15);
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(moveSpielfiguren()));
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(showInfizierte()));
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(showTote()));
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(showImmune()));
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(toDie()));
    connect(spielfigurTimer,SIGNAL(timeout()),this, SLOT(toRecover()));
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
    sterbensrate = newValue;
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
        startPosition = QPointF(zufallsZahl(30, width() - 30), zufallsZahl(30, height() - 30));
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

    for(Spielfigur *spielfigur : spielfigurList) {
        painter.save();
        painter.setPen(Qt::NoPen);
        if(spielfigur->isInfected())
            painter.setBrush(Qt::green);
        else if(!spielfigur->isAlive()){
            painter.setBrush(Qt::gray);
        }
        else if(spielfigur->isImmune()){
            painter.setBrush(Qt::blue);
        }
        else{
            painter.setBrush(Qt::black);}
        painter.drawEllipse(spielfigur->BoundingRect());
        painter.restore();
    }}

void CoronaField::moveSpielfiguren(){
    for(Spielfigur *spielfigur : spielfigurList) {
        if(!spielfigur->isAlive()||spielfigur->isIncubation()){
            spielfigur->removeActive();
        }
        spielfigur->move();
        if(spielfigur->isPos().y() + 10 > height() || spielfigur->isPos().y() - 10 < 0) {
            spielfigur->moveBack();
            spielfigur->changeSpeed(false);
            continue;
        }
        if(spielfigur->isPos().x() + 10 > width() || spielfigur->isPos().x() - 10 < 0) {
            spielfigur->moveBack();
            spielfigur->changeSpeed(true);
        }
        for(Spielfigur *kollision : spielfigurList) {
            if(spielfigur != kollision && (spielfigur->isPos() - kollision->isPos()).manhattanLength() < 22.5) {
                if((kollision->isAlive()&&spielfigur->isInfected()&&!kollision->isImmune())){
                    kollision->infect();}
                if(spielfigur->isAlive()&&kollision->isInfected()&&!spielfigur->isImmune()){
                    spielfigur->infect();}
                spielfigur->moveBack();
                spielfigur->changeDirection(qrand()%5);
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
        for(Spielfigur *spielfigur : spielfigurList) {
            spielfigur->simulationStarted();
        }
        spielfigurTimer->start();
    }
}

void CoronaField::stopSimulation(){
    spielfigurTimer->stop();
    updateTimer->stop();
    for(Spielfigur *spielfigur : spielfigurList) {
        spielfigur->simulationStopped();
    }
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
    this->Alive();
    this->setGesamtTote(0);
    this->setGesamtImmune(0);
    if(aufzeichnen){
        aufzeichnungsnummer++;
        /*if(!file.exists("Simulation.csv")){
            file.setFileName("Simulation.csv");
        }
        if(file.open(QIODevice::WriteOnly| QIODevice::Append | QIODevice::Text)) {

            QTextStream stream(&file);
            stream << "Messung, Zeit, Menschen, Aktiv, Infiziert, Immun, Tot" << endl;
            file.close();
        }*/

    }
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
            anzahl++;
        }
        if(!(GesamtInfizierte()==anzahl)){
            this->setGesamtInfizierte(anzahl);
        }
    }
    if(GesamtInfizierte()+GesamtTote()==ValueMenschen()||GesamtInfizierte()==ValueMenschen()||GesamtInfizierte()==0){
        stopSimulation();
        emit stop();
    }
}

void CoronaField::toDie(){
    double anzahl= round(GesamtInfizierte()*0.01*ValueSterben());
    int t = 0;
    for(int i = GesamtTote();i<anzahl;i++){
        if(spielfigurList.length()==i){
            return;
        }
        if(spielfigurList[t]->isInfected()&&!spielfigurList[t]->toDie()&&spielfigurList[t]->isAlive()){
            spielfigurList[t]->setToDie(true);
        }
        else if(!(spielfigurList[t]->isInfected)()||!(spielfigurList[t]->isAlive())){
            anzahl = anzahl+1;
        }
        t++;
    }
}

void CoronaField::toRecover(){
    for(Spielfigur *spielfigur : spielfigurList) {
        if(spielfigur->isInfected()&&!spielfigur->toRecover()&&!spielfigur->toDie()){
            spielfigur->setToRecover(true);
        }
    }
}

void CoronaField::Alive(){
    for(Spielfigur *spielfigur : spielfigurList) {
        spielfigur->Alive();
    }
}

void CoronaField::setGesamtTote(int anzahl){
    gesamtTote = anzahl;
    emit tot();
}

int CoronaField::GesamtTote(){
    return gesamtTote;
}

void CoronaField::showTote(){
    int anzahl = 0;
    for(Spielfigur *spielfigur : spielfigurList) {
        if(!spielfigur->isAlive()){
            anzahl=anzahl+1;
        }
        if(!(GesamtTote()==anzahl)){
            this->setGesamtTote(anzahl);
        }
    }
    if(GesamtTote()+GesamtInfizierte()==ValueMenschen()||anzahl==ValueMenschen()){
        stopSimulation();
        emit stop();
    }
}

void CoronaField::setGesamtImmune(int anzahl){
    gesamtImmune = anzahl;
    emit immune();
}

int CoronaField::GesamtImmune(){
    return gesamtImmune;
}

void CoronaField::showImmune(){
    int anzahl = 0;
    for(Spielfigur *spielfigur : spielfigurList) {
        if(spielfigur->isImmune()){
            anzahl=anzahl+1;
        }
        if(!(GesamtImmune()==anzahl)){
            this->setGesamtImmune(anzahl);
        }
    }
}

void CoronaField::setSimAufzeichen(bool newValue){
    aufzeichnen = newValue;
    if(aufzeichnen){

    }
}

bool CoronaField::simAufzeichnen(){
    return aufzeichnen;
}

void CoronaField::record(QString currentTime){
    if(!aufzeichnen){
        return;
    }
    if(!file.exists("Simulation.csv")){
        file.setFileName("Simulation.csv");
        if (file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text)) {
            QTextStream stream(&file);
            stream << "Messung, Zeit, Menschen, Aktiv, Infiziert, Immun, Tot" << endl;
            file.close();
        }
    }
    if (file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text)) {
        QTextStream stream(&file);
        stream << QString::number(aufzeichnungsnummer) + ", " + currentTime + ", " + QString::number(ValueMenschen()) + ", " + QString::number(ValueAktive()) + ", " + QString::number(GesamtInfizierte()) + ", " + QString::number(GesamtImmune()) + ", " + QString::number(GesamtTote()) << endl;
        file.close();
    }
}

void CoronaField::fileExport(){
    if(!QFile::exists("Simulation.csv")){
        return;
    }
    QFileDialog fileDialog;
    QString fname = fileDialog.getSaveFileName(this, tr("Safe File"), "/home/untitled.csv", tr("Text files (*.csv)"));
    if(fname.isEmpty()){
        return;
    }
    if(QFile::exists(fname)){
        QFile::remove(fname);
    }
    file.copy(fname);
}
