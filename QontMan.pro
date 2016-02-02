#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T11:30:46
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QontMan
TEMPLATE = app


SOURCES +=  src\main.cpp\
            src\mainwindow.cpp \
            src\PlainDb.cpp \
            src\datamodel.cpp \
    src/contact.cpp \
    src/contview.cpp \
    src/treemodel.cpp \
    src/diagram.cpp \
    src/dia/edge.cpp \
    src/dia/graphwidget.cpp \
    src/dia/node.cpp \
    src/dia/gnodeinfo.cpp

HEADERS  += src\mainwindow.h \
            src\contact.h \
            src\PlainDb.h \
            src\datamodel.h \
    src/contview.h \
    src/treemodel.h \
    src/nodeinfo.h \
    src/diagram.h \
    src/dia/edge.h \
    src/dia/graphwidget.h \
    src/dia/node.h \
    src/dia/gnodeinfo.h

FORMS    += src\mainwindow.ui \
    src/contview.ui \
    src/diagram.ui

CONFIG   += c++11

DISTFILES +=

RESOURCES += \
    res.qrc
