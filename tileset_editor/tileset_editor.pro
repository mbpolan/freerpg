#-------------------------------------------------
#
# Project created by QtCreator 2012-05-30T22:18:10
#
#-------------------------------------------------

QT       += core gui
MOC_DIR = moc
UI_DIR = src/ui

TARGET = tileset_editor
TEMPLATE = app


SOURCES += \
    src/mainwindow.cpp \
    src/main.cpp \
    src/tileeditor.cpp \
    src/newdialog.cpp \
    src/tileset.cpp \
    src/tile.cpp \
    src/iotileset.cpp \
    src/propertiesdialog.cpp

HEADERS  += \
    src/mainwindow.h \
    src/tileeditor.h \
    src/newdialog.h \
    src/tileset.h \
    src/tile.h \
    src/iotileset.h \
    src/propertiesdialog.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/newdialog.ui \
    src/ui/propertiesdialog.ui
