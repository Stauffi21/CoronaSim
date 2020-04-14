#include "gameboard.h"
#include "ui_gameboard.h"
#include "coronafield.h"

gameboard::gameboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameboard)
{
    ui->setupUi(this);
    CoronaField = new coronaField;
    ui->gridLayout->addWidget(coronaField);
}

gameboard::~gameboard()
{
    delete ui;
}

void gameboard::start(){}
void gameboard::stop(){}
void gameboard::reset(){}
