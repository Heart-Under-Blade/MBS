#-------------------------------------------------
#
# Project created by QtCreator 2017-09-21T12:11:20
#
#-------------------------------------------------

QT       += core gui widgets charts
TEMPLATE = app
CONFIG += c++11

DESTDIR = ../bin
#CONFIG+=wwwidgets

LIBS += -lopengl32 -lglu32 -lglut32cu

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=gnu++11
QMAKE_CXXFLAGS += -march=corei7 -msse4.2

TEMPLATE = app

CONFIG(release, debug|release): {
    TARGET = BS_visual
}

CONFIG(debug, debug|release): {
    DEFINES += _DEBUG
    TARGET = BS_visual_dd
}

DEFINES += MODE_FIXED_OR

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SRC = ../../BS_Lib/src
include(../../BS_Lib/pro/MBS.pri)

INCLUDEPATH += \
    ../src \
    ../src/ui

FORMS += \
        ../src/ui/*.ui
#        ../src/ui/MainWindow.ui \
#        ../src/ui/SettingsDialog.ui

SOURCES += \
    ../src/*.cpp \
        ../src/ui/*.cpp
#        ../src/ui/MainWindow.cpp \
#        ../src/ui/SettingsDialog.cpp

HEADERS += \
    ../src/*.h \
        ../src/ui/*.h
#        ../src/ui/MainWindow.h \
#        ../src/ui/SettingsDialog.h


RESOURCES += \
    *.qrc
