QT += core network gui widgets

TARGET = QtClient
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += debug_and_release

SOURCES += \
    clientmain.cpp \
    chatwindow.cpp \
    chatclient.cpp \
    ../QtServer/qtchatuser.cpp

FORMS += \
    chatwindow.ui

HEADERS += \
    chatwindow.h \
    chatclient.h \
    ../QtServer/qtchatuser.h
