QT += \
    qml \
    quick \
    3dcore \
    3drender \
    3dinput \
    3dquick \
    3dextras

CONFIG += c++11

SOURCES += main.cpp \
    Scene.cpp \
    Camera.cpp \
    Application.cpp \
    Object.cpp \
    Point.cpp \
    Polygon.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    Scene.h \
    Camera.h \
    Application.h \
    Object.h \
    Point.h \
    Polygon.h
