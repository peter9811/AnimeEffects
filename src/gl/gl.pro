include(../common.pri)

TARGET = gl
TEMPLATE = lib
CONFIG += staticlib

LIBS += -L"$$OUT_PWD/../util/" -lutil

INCLUDEPATH += ..

SOURCES += \
    ExtendShader.cpp \
    Global.cpp \
    Task.cpp \
    Texture.cpp \
    BufferObject.cpp \
    EasyShaderProgram.cpp \
    Util.cpp \
    ComputeTexture1D.cpp \
    DeviceInfo.cpp \
    Framebuffer.cpp \
    EasyTextureDrawer.cpp \
    VertexArrayObject.cpp \
    Root.cpp \
    PrimitiveDrawer.cpp \
    Triangulator.cpp \
    FontDrawer.cpp \
    TextObject.cpp

HEADERS += \
    EasyShaderProgram.h \
    ExtendShader.h \
    Global.h \
    Util.h \
    Task.h \
    Texture.h \
    Vector3.h \
    Vector2.h \
    Vector4.h \
    BufferObject.h \
    Vector2I.h \
    Vector4I.h \
    ComputeTexture1D.h \
    DeviceInfo.h \
    Framebuffer.h \
    EasyTextureDrawer.h \
    VertexArrayObject.h \
    Root.h \
    PrimitiveDrawer.h \
    Triangulator.h \
    FontDrawer.h \
    TextObject.h
