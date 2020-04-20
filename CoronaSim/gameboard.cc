#include "gameboard.h"
#include "ui_gameboard.h"
#include "coronafield.h"

gameboard::gameboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameboard)
{
    ui->setupUi(this);
    ui->infizierte->setDisabled(true);
    ui->aktive->setDisabled(true);
    pCoronaField = new CoronaField();
    ui->stoppwatch->setText(pCoronaField->isStoppwatch());
    ui->gridLayout->addWidget(pCoronaField);
    connect(ui->start,SIGNAL(clicked()),this,SLOT(startSimulation()));
    //connect(ui->stop,SIGNAL(clicked()),this,SLOT(stopSimulation()));
    connect(ui->menschen,SIGNAL(valueChanged(int)),this, SLOT(anzMenschen(int)));
    connect(ui->infizierte,SIGNAL(valueChanged(int)),this, SLOT(anzInfizierte(int)));
    connect(ui->aktive,SIGNAL(valueChanged(int)),this, SLOT(anzAktive(int)));
}

gameboard::~gameboard()
{
    delete ui;
}

void gameboard::stop(){}

void gameboard::reset(){}

void gameboard::anzMenschen(int i){
    int newValue=i;
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
    }
    ui->infizierte->setValue(pCoronaField->ValueInfizierte());
}

void gameboard::anzInfizierte(int i){
    int newValue = i;
    pCoronaField->setValueInfizierte(newValue);
    ui->infizierte->setValue(pCoronaField->ValueInfizierte());
}

void gameboard::anzAktive(int i){
    int newValue = i;
    pCoronaField->setValueAktive(newValue);
}

void gameboard::startSimulation(){
    pCoronaField->startSimulation();
}
