QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stl_viewer
TEMPLATE = app

CONFIG += c++11

HEADERS += \
    src/camera.h \
    src/canvas.h \
    src/mainwindow.h \
    src/model.h \
    src/stl.h \
    src/types.h

SOURCES += \
    src/camera.cpp \
    src/canvas.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/model.cpp \
    src/stl.cpp \
    src/types.cpp

DISTFILES += \
    LICENSE \
    README.md
