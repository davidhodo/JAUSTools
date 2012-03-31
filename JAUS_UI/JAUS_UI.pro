#-------------------------------------------------
#
# Project created by QtCreator 2012-03-31T15:39:12
#
#-------------------------------------------------

QT       += core gui

TARGET = JAUS_UI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    qdebugstream.h

FORMS    += mainwindow.ui

LIBS += -lopenjaus\
        -lopenjaus-core\
        -lopenjaus-mobility
