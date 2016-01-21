#-------------------------------------------------
#
# Project created by QtCreator 2015-12-02T01:33:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Event_Based_Control
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scene.cpp \
    controller.cpp \
    simulation.cpp

HEADERS  += mainwindow.h \
    scene.h \
    controller.h \
    simulation.h \
    trans.hpp

FORMS    += mainwindow.ui

RESOURCES += \
    graphics.qrc

CONFIG += c++14
