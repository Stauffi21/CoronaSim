#include "gameboard.h"
#include "ui_gameboard.h"
#include "coronafield.h"

gameboard::gameboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameboard)
{
    ui->setupUi(this);
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
    pCoronaField->valueMenschen(newValue);
}
void gameboard::anzInfizierte(int i){
    int newValue = i;
    pCoronaField->valueInfizierte(newValue);
}
void gameboard::anzAktive(int i){
    int newValue = i;
    pCoronaField->valueAktive(newValue);
}
