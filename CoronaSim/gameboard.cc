#include "gameboard.h"
#include "ui_gameboard.h"
#include "coronafield.h"

gameboard::gameboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameboard)
{
    ui->setupUi(this);
    ui->infizierte->setDisabled(true);
    pCoronaField = new CoronaField();
    ui->gridLayout->addWidget(pCoronaField);
    connect(ui->menschen,SIGNAL(valueChanged(int)),this, SLOT(anzMenschen(int)));
    connect(ui->infizierte,SIGNAL(valueChanged(int)),this, SLOT(anzInfizierte(int)));
    connect(ui->aktive,SIGNAL(valueChanged(int)),this, SLOT(anzAktive(int)));
}

gameboard::~gameboard()
{
    delete ui;
}

void gameboard::start(){}
void gameboard::stop(){}
void gameboard::reset(){}
void gameboard::anzMenschen(int i){
    int newValue=i;
    pCoronaField->setValueMenschen(newValue);
    if(pCoronaField->ValueMenschen()<pCoronaField->ValueInfizierte()){
        ui->infizierte->setDisabled(true);
    }
    else{
        ui->infizierte->setEnabled(true);
    }
    ui->infizierte->setValue(pCoronaField->ValueInfizierte());
}
void gameboard::anzInfizierte(int i){
    int newValue = i;
    if(pCoronaField->ValueMenschen()<newValue){
        ui->infizierte->setDisabled(true);
    }
    else{
        ui->infizierte->setEnabled(true);
    }
    pCoronaField->setValueInfizierte(newValue);
    ui->infizierte->setValue(pCoronaField->ValueInfizierte());
}
void gameboard::anzAktive(int i){
    int newValue = i;
    pCoronaField->setValueAktive(newValue);
}
