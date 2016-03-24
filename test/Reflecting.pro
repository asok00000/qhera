#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T10:25:13
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Reflecting
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../qhera.cpp \
    birdmodel.cpp

HEADERS  += mainwindow.h \
    ../qhera.h \
    birdmodel.h

FORMS    += mainwindow.ui
