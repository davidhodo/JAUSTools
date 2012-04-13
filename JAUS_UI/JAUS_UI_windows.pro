#-------------------------------------------------
#
# Project created by QtCreator 2012-03-31T15:39:12
#
#-------------------------------------------------

QT       += core gui

TARGET = JAUS_UI
TEMPLATE = app

INCLUDEPATH +=  C:\Users\HodoD\Documents\Development\com.sdk\include

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

LIBS += C:\Users\HodoD\Documents\Development\com.sdk\libopenjaus\
        C:\Users\HodoD\Documents\Development\com.sdk\libopenjaus-core\
        C:\Users\HodoD\Documents\Development\com.sdk\libopenjaus-mobility

QMAKE_CXXFLAGS += /Zc:wchar_t
