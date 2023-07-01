include(../common.pri)

TARGET = img
TEMPLATE = lib
CONFIG += staticlib

LIBS += -L"$$OUT_PWD/../util/" -lutil

INCLUDEPATH += ..

SOURCES += \
    PSDReader.cpp \
    PSDUtil.cpp \
    PSDWriter.cpp \
    Buffer.cpp \
    ResourceNode.cpp \
    Util.cpp \
    BlendMode.cpp \
    GridMeshCreator.cpp \
    ResourceData.cpp \
    ResourceHandle.cpp \
    BlendModeName.cpp

HEADERS += \
    Buffer.h \
    PSDFormat.h \
    PSDReader.h \
    PSDUtil.h \
    PSDWriter.h \
    Format.h \
    PixelPos.h \
    Quad.h \
    ResourceNode.h \
    ResourceHandle.h \
    Util.h \
    ColorRGBA.h \
    BlendMode.h \
    GridMeshCreator.h \
    ResourceData.h \
    BlendModeName.h
