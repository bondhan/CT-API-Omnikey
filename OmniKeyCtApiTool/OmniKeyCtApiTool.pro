#-------------------------------------------------
#
# Project created by QtCreator 2017-02-07T11:20:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OmniKeyCtApiTool
TEMPLATE = app
INCLUDEPATH += "C:\Omnikey\CT-API_V4_0_2_2A\Development"

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:LIBS += "C:\Omnikey\CT-API_V4_0_2_2A\Development\ctdeutin.lib"
