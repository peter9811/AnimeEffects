include(../common.pri)

TARGET = thr
TEMPLATE = lib
CONFIG += staticlib

LIBS += -L"$$OUT_PWD/../util/" -lutil

INCLUDEPATH += ..

SOURCES += \
    Worker.cpp \
    TaskQueue.cpp \
    Task.cpp \
    Paralleler.cpp

HEADERS += \
    Worker.h \
    TaskQueue.h \
    Task.h \
    Paralleler.h
