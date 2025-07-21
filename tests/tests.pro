QT += testlib core gui
CONFIG += console c++11
TARGET = testrunner
SOURCES += test_state.cpp \
    ../state.cpp
HEADERS += \
    ../state.h
