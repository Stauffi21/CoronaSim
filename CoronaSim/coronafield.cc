#include "coronafield.h"
#include "gameboard.h"

#include <QDateTime>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>

#include <math.h>
#include <stdlib.h>

CoronaField::CoronaField(QWidget *parent) : QWidget(parent)
{

}

void CoronaField::valueMenschen(int newValue){
    int value = newValue;
    qDebug() << value;
}

void CoronaField::valueInfizierte(int newValue){
    int value = newValue;
    qDebug() << value;
}

void CoronaField::valueAktive(int newValue){
    int value = newValue;
    qDebug() << value;
}
