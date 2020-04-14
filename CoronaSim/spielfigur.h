#ifndef SPIELFIGUR_H
#define SPIELFIGUR_H

#include <QObject>

class spielfigur : public QObject
{
    Q_OBJECT
public:
    explicit spielfigur(QObject *parent = 0);

signals:

public slots:
};

#endif // SPIELFIGUR_H
