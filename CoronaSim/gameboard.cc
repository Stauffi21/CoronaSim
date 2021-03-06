#include "gameboard.h"
#include "ui_gameboard.h"
#include "coronafield.h"
#include <QTime>
#include <QString>
#include <QDebug>
#include <QFileDialog>

gameboard::gameboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameboard)
{
    ui->setupUi(this);
    pCoronaField = new CoronaField();
    ui->gridLayout->addWidget(pCoronaField);
    time = new QTime(0,0);
    elapsedTimer = new QElapsedTimer();
    simulationStopped = 0;
    timer = new QTimer();
    timer->setInterval(10);
    recordTimer = new QTimer();
    recordTimer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    connect(recordTimer, SIGNAL(timeout()), this, SLOT(saveData()));
    connect(ui->start,SIGNAL(clicked()),this,SLOT(startSimulation()));
    connect(ui->stop,SIGNAL(clicked()),this,SLOT(stopSimulation()));
    connect(ui->reset,SIGNAL(clicked()),this, SLOT(resetSimulation()));
    connect(ui->menschen,SIGNAL(valueChanged(int)),this, SLOT(anzMenschen(int)));
    connect(ui->infizierte,SIGNAL(valueChanged(int)),this, SLOT(anzInfizierte(int)));
    connect(ui->aktive,SIGNAL(valueChanged(int)),this, SLOT(anzAktive(int)));
    connect(ui->sterbensrate,SIGNAL(valueChanged(double)),this, SLOT(anzSterben(double)));
    connect(ui->aufzeichnen,SIGNAL(stateChanged(int)),this, SLOT(aufzeichnenSimulation(int)));
    connect(ui->export_2,SIGNAL(clicked()),this, SLOT(isExport()));
    connect(pCoronaField,SIGNAL(infziert()),this, SLOT(showInfizierte()));
    connect(pCoronaField,SIGNAL(tot()),this, SLOT(showTote()));
    connect(pCoronaField,SIGNAL(immune()),this, SLOT(showImmune()));
    connect(pCoronaField,SIGNAL(stop()),this, SLOT(stopSimulation()));
    pCoronaField->setValueSterben(ui->sterbensrate->value());
    pCoronaField->setValueMenschen(ui->menschen->value());
    pCoronaField->setValueInfizierte(ui->infizierte->value());
    pCoronaField->setGesamtInfizierte(1);
    pCoronaField->setGesamtTote(0);
    pCoronaField->setGesamtImmune(0);
    pCoronaField->setValueAktive(ui->aktive->value());
    pCoronaField->setSimAufzeichen(false);
}

gameboard::~gameboard()
{
    delete ui;
}

void gameboard::anzMenschen(int newValue){
    pCoronaField->setValueMenschen(newValue);
    pCoronaField->setValueAktive(pCoronaField->ValueAktive());
    if(pCoronaField->ValueMenschen()==pCoronaField->ValueInfizierte()){
        ui->start->setDisabled(true);
    }
    else if(pCoronaField->ValueMenschen()==0){
        ui->infizierte->setDisabled(true);
        ui->aktive->setDisabled(true);
        ui->start->setDisabled(true);
    }
    else{
        ui->infizierte->setEnabled(true);
        ui->aktive->setEnabled(true);
        ui->start->setEnabled(true);
    }
    ui->infizierte->setValue(pCoronaField->ValueInfizierte());
}

void gameboard::anzInfizierte(int newValue){
    pCoronaField->setValueInfizierte(newValue);
    ui->infizierte->setValue(pCoronaField->ValueInfizierte());
    ui->anz_Infizierte->setText(QString::number(pCoronaField->ValueInfizierte()));
    if(pCoronaField->ValueMenschen()==pCoronaField->ValueInfizierte()){
        ui->start->setDisabled(true);
    }
    else{
        ui->start->setDisabled(false);
    }
}

void gameboard::anzAktive(int newValue){
    pCoronaField->setValueAktive(newValue);
}

void gameboard::anzSterben(double newValue){
    pCoronaField->setValueSterben(newValue);
}
void gameboard::startSimulation(){
    if(pCoronaField->simAufzeichnen()){
        recordTimer->start();
    }
    pCoronaField->startSimulation();
    timer->start();
    elapsedTimer->start();
}

void gameboard::stopSimulation(){
    simulationStopped = simulationStopped+elapsedTimer->elapsed();
    pCoronaField->stopSimulation();
    timer->stop();
    recordTimer->stop();
}

void gameboard::resetSimulation(){
    timer->stop();
    recordTimer->stop();
    simulationStopped=0;
    ui->stoppwatch->setText("00:00:000");
    pCoronaField->resetSimulation(ui->menschen->value(), ui->infizierte->value(), ui->aktive->value());
}

void gameboard::showTime()
{
    QString text = time->fromMSecsSinceStartOfDay(elapsedTimer->elapsed()+simulationStopped).toString("mm:ss:zzz");
    ui->stoppwatch->setText(text);
}

void gameboard::showInfizierte(){
    QString infizierte = QString::number(pCoronaField->GesamtInfizierte());
    ui->anz_Infizierte->setText(infizierte);
}

void gameboard::showTote(){
    QString tote = QString::number(pCoronaField->GesamtTote());
    ui->anz_Tote->setText(tote);
}

void gameboard::showImmune(){
    QString immune = QString::number(pCoronaField->GesamtImmune());
    ui->anz_Immune->setText(immune);
}

void gameboard::aufzeichnenSimulation(int state){
    //qDebug() << state;
    if(state == 0){
        pCoronaField->setSimAufzeichen(false);
    }

    else if(state == 2){
        pCoronaField->setSimAufzeichen(true);
    }
}

void gameboard::isExport(){
    this->stopSimulation();
    pCoronaField->fileExport();
}

void gameboard::saveData(){
    pCoronaField->record(ui->stoppwatch->text());
}
