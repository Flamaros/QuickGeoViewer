QT += \
    qml \
    quick \
    3dcore \
    3drender \
    3dinput \
    3dquick \
    3dextras

CONFIG += c++11

SOURCES += \
    ../source/main.cpp \
    ../source/Scene.cpp \
    ../source/Camera.cpp \
    ../source/Application.cpp \
    ../source/Object.cpp \
    ../source/Point.cpp \
    ../source/Polygon.cpp

RESOURCES += ../data/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    ../source/Scene.h \
    ../source/Camera.h \
    ../source/Application.h \
    ../source/Object.h \
    ../source/Point.h \
    ../source/Polygon.h
