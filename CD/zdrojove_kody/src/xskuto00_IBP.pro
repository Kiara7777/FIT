#-------------------------------------------------
#
# Project created by QtCreator 2017-03-03T16:31:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xskuto00_IBP
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    fotofile.cpp \
    skupinydialog.cpp \
    foto.cpp \
    fotoitemwidget.cpp \
    exifitemwidget.cpp \
    exifpanel.cpp \
    exifdateitemwidget.cpp \
    skupina.cpp \
    skupinyitemwidget.cpp \
    allfoto.cpp \
    duplicitnifoto.cpp \
    duplicita.cpp \
    duplikatyitemwidget.cpp \
    duplikatyskupitemwidget.cpp \
    fotowidget.cpp \
    fotopicturewidget.cpp \
    duplikatyskuppicturewidget.cpp \
    mexif.cpp

win32 {
    INCLUDEPATH  += $$quote(C:/MinGW/msys/1.0/local/include)
    LIBS         += $$quote(C:/MinGW/msys/1.0/local/lib/libexiv2.dll.a)
}

HEADERS  += mainwindow.h \
    fotofile.h \
    skupinydialog.h \
    foto.h \
    fotoitemwidget.h \
    exifitemwidget.h \
    exifpanel.h \
    exifdateitemwidget.h \
    skupina.h \
    skupinyitemwidget.h \
    allfoto.h \
    duplicitnifoto.h \
    duplicita.h \
    duplikatyitemwidget.h \
    duplikatyskupitemwidget.h \
    fotowidget.h \
    fotopicturewidget.h \
    duplikatyskuppicturewidget.h \
    mexif.h
