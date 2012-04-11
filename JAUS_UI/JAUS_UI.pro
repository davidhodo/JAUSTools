#-------------------------------------------------
#
# Project created by QtCreator 2012-03-31T15:39:12
#
#-------------------------------------------------

QT       += core gui

TARGET = JAUS_UI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    joystick.cpp \
    jausclient.cpp

HEADERS  += mainwindow.h \
    qdebugstream.h \
    joystick.h \
    jausclient.h \
    openjaus_structures.h

FORMS    += mainwindow.ui

LIBS += -lopenjaus\
        -lopenjaus-core\
        -lopenjaus-mobility
