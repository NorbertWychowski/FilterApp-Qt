#-------------------------------------------------
#
# Project created by QtCreator 2017-11-02T14:54:10
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FilterApp
TEMPLATE = app

CONFIG += C++11
CONFIG += static
CONFIG -= embed_manifest_exe

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


win32:RC_ICONS += icon.ico

VPATH += /filters/

FORMS += \
    tools/histogram.ui \
    colormenu.ui \
    custommaskdialog.ui \
    filtersmenu.ui \
    mainwindow.ui

HEADERS += \
    tools/histogram.h \
    filters/boxblur.h \
    filters/colortool.h \
    filters/filtertool.h \
    filters/gaussianblur.h \
    filters/math/labconverter.h \
    filters/math/matrix.h \
    colormenu.h \
    custommaskdialog.h \
    filtergraphicsview.h \
    filtersmenu.h \
    mainwindow.h \
    mygraphicsview.h \
    selectiontool.h

SOURCES += \
    tools/histogram.cpp \
    filters/boxblur.cpp \
    filters/colortool.cpp \
    filters/filtertool.cpp \
    filters/gaussianblur.cpp \
    filters/math/labconverter.cpp \
    filters/math/matrix.cpp \
    colormenu.cpp \
    custommaskdialog.cpp \
    filtergraphicsview.cpp \
    filtersmenu.cpp \
    main.cpp \
    mainwindow.cpp \
    mygraphicsview.cpp \
    selectiontool.cpp
