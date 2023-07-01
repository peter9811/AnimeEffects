include(../common.pri)

TARGET = core
TEMPLATE = lib
CONFIG += staticlib

LIBS += \
    -L"$$PWD/../img/"  -limg \
    -L"$$PWD/../gl/"   -lgl \
    -L"$$PWD/../cmnd/" -lcmnd \
    -L"$$PWD/../thr/"  -lthr \
    -L"$$PWD/../util/" -lutil

INCLUDEPATH += ..

SOURCES += \
    AbstractCursor.cpp \
    CameraInfo.cpp \
    GridMesh.cpp \
    HSVKey.cpp \
    LayerNode.cpp \
    ObjectNodeUtil.cpp \
    ObjectTree.cpp \
    Project.cpp \
    TimeLine.cpp \
    FFDKey.cpp \
    HeightMap.cpp \
    Deserializer.cpp \
    Serializer.cpp \
    BoneKey.cpp \
    Bone2.cpp \
    PoseKey.cpp \
    PosePalette.cpp \
    ObjectTreeEvent.cpp \
    ObjectTreeNotifier.cpp \
    TimeKey.cpp \
    BoneShape.cpp \
    TimeKeyGatherer.cpp \
    BoneInfluenceMap.cpp \
    TimeKeyBlender.cpp \
    TimeKeyExpans.cpp \
    MeshTransformer.cpp \
    ResourceHolder.cpp \
    OpaKey.cpp \
    MeshKey.cpp \
    LayerMesh.cpp \
    ResourceEvent.cpp \
    FFDKeyUpdater.cpp \
    BoneKeyUpdater.cpp \
    ClippingFrame.cpp \
    ShaderHolder.cpp \
    TimeCacheLock.cpp \
    TimeCacheAccessor.cpp \
    TimeLineEvent.cpp \
    MeshKeyUtil.cpp \
    ProjectEvent.cpp \
    MeshTransformerResource.cpp \
    ImageKey.cpp \
    ImageKeyUpdater.cpp \
    ResourceUpdatingWorkspace.cpp \
    BoneExpans.cpp \
    FolderNode.cpp \
    DestinationTexturizer.cpp \
    MoveKey.cpp \
    RotateKey.cpp \
    ScaleKey.cpp \
    SRTExpans.cpp \
    DepthKey.cpp \
    TimeFormat.cpp

HEADERS += \
    AbstractCursor.h \
    CameraInfo.h \
    HsvKey.h \
    TimeKeyType.h \
    GridMesh.h \
    LayerNode.h \
    ObjectNode.h \
    ObjectNodeUtil.h \
    ObjectTree.h \
    Project.h \
    Renderer.h \
    RenderInfo.h \
    TimeInfo.h \
    TimeKey.h \
    TimeLine.h \
    Animator.h \
    TimeLineEvent.h \
    TimeKeyPos.h \
    Constant.h \
    FFDKey.h \
    ObjectType.h \
    HeightMap.h \
    Serializer.h \
    Deserializer.h \
    BoneKey.h \
    Bone2.h \
    PoseKey.h \
    PosePalette.h \
    ObjectTreeEvent.h \
    ObjectTreeNotifier.h \
    BoneShape.h \
    TimeKeyGatherer.h \
    BoneInfluenceMap.h \
    TimeKeyBlender.h \
    TimeKeyExpans.h \
    SRTExpans.h \
    MeshTransformer.h \
    ResourceHolder.h \
    OpaKey.h \
    MeshKey.h \
    LayerMesh.h \
    ResourceEvent.h \
    FFDKeyUpdater.h \
    BoneKeyUpdater.h \
    ClippingFrame.h \
    ShaderHolder.h \
    TimeCacheLock.h \
    TimeCacheAccessor.h \
    Frame.h \
    MeshKeyUtil.h \
    ProjectEvent.h \
    MeshTransformerResource.h \
    ImageKey.h \
    ImageKeyUpdater.h \
    ResourceUpdatingWorkspace.h \
    BoneExpans.h \
    FolderNode.h \
    DestinationTexturizer.h \
    MoveKey.h \
    RotateKey.h \
    ScaleKey.h \
    DepthKey.h\
    TimeFormat.h
