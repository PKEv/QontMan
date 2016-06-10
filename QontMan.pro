QT       += core gui sql widgets

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
            src/passport.cpp \
    src/vcard.cpp

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
            src/passport.h \
    src/vcard.h

FORMS    += src\mainwindow.ui \
            src/contview.ui \
            src/diagram.ui \
            src/passport.ui

CONFIG   += c++11

DISTFILES +=

RESOURCES += \
            res.qrc
RC_FILE     += resources.rc

INCLUDEPATH += "c:\Firebird\include"

LIBS+=-LC:\Firebird\bin -lfbclient
