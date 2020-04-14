#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>

class CoronaField;

namespace Ui {
class gameboard;
}

class gameboard : public QWidget
{
    Q_OBJECT

public:
    explicit gameboard(QWidget *parent = 0);
    ~gameboard();

private:
    Ui::gameboard *ui;
    CoronaField *coronaField;

private slots:
    void start();
    void stop();
    void reset();
};

#endif // GAMEBOARD_H
