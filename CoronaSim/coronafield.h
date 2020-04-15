#ifndef CORONAFIELD_H
#define CORONAFIELD_H

#include <QWidget>

class CoronaField : public QWidget
{
    Q_OBJECT
public:
    CoronaField(QWidget *parent = 0);
    void valueMenschen(int newValue);
    void valueInfizierte(int newValue);
    void valueAktive(int newValue);

signals:

public slots:
};

#endif // CORONAFIELD_H
