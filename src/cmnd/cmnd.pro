include(../common.pri)

TARGET = cmnd
TEMPLATE = lib
CONFIG += staticlib

LIBS += -L"$$PWD/../util" -lutil

INCLUDEPATH += ..

SOURCES += \
    Stack.cpp \
    Scalable.cpp

HEADERS += \
    Base.h \
    BasicCommands.h \
    Listener.h \
    ScopedMacro.h \
    ScopedUndoSuspender.h \
    SleepableObject.h \
    Stack.h \
    SignalNotifier.h \
    Scalable.h \
    UndoneDeleter.h \
    DoneDeleter.h \
    Stable.h \
    Vector.h
