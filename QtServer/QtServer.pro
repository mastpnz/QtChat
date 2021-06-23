QT += core network gui widgets sql

TARGET = QtServer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += debug_and_release

HEADERS += \
    chatserver.h \
    dialogaddmodifyuser.h \
    dialogusermanage.h \
    qtchatuser.h \
    serverworker.h \
    serverwindow.h \
    startserverdialog.h

SOURCES += \
    chatserver.cpp \
    dialogaddmodifyuser.cpp \
    dialogusermanage.cpp \
    qtchatuser.cpp \
    servermain.cpp \
    serverworker.cpp \
    serverwindow.cpp \
    startserverdialog.cpp

FORMS += \
    dialogaddmodifyuser.ui \
    dialogusermanage.ui \
    serverwindow.ui \
    startserverdialog.ui
