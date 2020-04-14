#-------------------------------------------------
#
# Project created by QtCreator 2020-04-14T15:40:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CoronaSim
TEMPLATE = app


SOURCES += main.cpp\
        gameboard.cc \
    coronafield.cc \
    spielfigur.cc

HEADERS  += gameboard.h \
    coronafield.h \
    spielfigur.h

FORMS    += gameboard.ui
