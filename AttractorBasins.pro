#-------------------------------------------------
#
# Project created by QtCreator 2016-10-31T17:04:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AttractorBasins
TEMPLATE = app
LIBS += -lGLU


SOURCES += main.cpp \
    mainwindow.cpp \
    graph.cpp

HEADERS  += \
    mainwindow.h \
    graph.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
