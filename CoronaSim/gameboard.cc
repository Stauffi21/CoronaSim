#include "gameboard.h"
#include "ui_gameboard.h"
#include "coronafield.h"
#include <QTime>
#include <QString>

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
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    connect(ui->start,SIGNAL(clicked()),this,SLOT(startSimulation()));
    connect(ui->stop,SIGNAL(clicked()),this,SLOT(stopSimulation()));
    connect(ui->reset,SIGNAL(clicked()),this, SLOT(resetSimulation()));
    connect(ui->menschen,SIGNAL(valueChanged(int)),this, SLOT(anzMenschen(int)));
    connect(ui->infizierte,SIGNAL(valueChanged(int)),this, SLOT(anzInfizierte(int)));
    connect(ui->aktive,SIGNAL(valueChanged(int)),this, SLOT(anzAktive(int)));
    connect(ui->sterbensrate,SIGNAL(valueChanged(double)),this, SLOT(anzSterben(double)));
    connect(pCoronaField,SIGNAL(infziert()),this, SLOT(showInfizierte()));
    connect(pCoronaField,SIGNAL(stop()),this, SLOT(stopSimulation()));
    pCoronaField->setValueSterben(3.2);
    pCoronaField->setValueMenschen(4);
    pCoronaField->setValueInfizierte(1);
    pCoronaField->setGesamtInfizierte(1);
    pCoronaField->setValueAktive(20);
}

gameboard::~gameboard()
{
    delete ui;
}

void gameboard::anzMenschen(int newValue){
    pCoronaField->setValueMenschen(newValue);
    pCoronaField->setValueAktive(pCoronaField->ValueAktive());
    if(pCoronaField->ValueMenschen()==0){
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
}

void gameboard::anzAktive(int newValue){
    pCoronaField->setValueAktive(newValue);
}

void gameboard::anzSterben(double newValue){
    pCoronaField->setValueSterben(newValue);
}
void gameboard::startSimulation(){
    pCoronaField->startSimulation();
    timer->start();
    elapsedTimer->start();
}

void gameboard::stopSimulation(){
    simulationStopped = simulationStopped+elapsedTimer->elapsed();
    pCoronaField->stopSimulation();
    timer->stop();
}

void gameboard::resetSimulation(){
    timer->stop();
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
