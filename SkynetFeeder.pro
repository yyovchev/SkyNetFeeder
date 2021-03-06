#-------------------------------------------------
#
# Project created by QtCreator 2016-07-13T13:26:54
#
#-------------------------------------------------

QT       += core gui
QT       += sql network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    radar.cpp \
    aircraftsdb.cpp \
    binaryparser.cpp \
    localreader.cpp \
    oevalues.cpp \
    task.cpp \
    aircrafts.cpp \
    locations.cpp \
    datareader.cpp \
    datasender.cpp \
    qryreader.cpp \
    filewriter.cpp

HEADERS  += mainwindow.h \
    radar.h \
    defines.h \
    aircraftsdb.h \
    binaryparser.h \
    localreader.h \
    oevalues.h \
    task.h \
    aircrafts.h \
    locations.h \
    datareader.h \
    datasender.h \
    qryreader.h \
    filewriter.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
