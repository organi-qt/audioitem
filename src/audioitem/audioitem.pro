QT += qml quick

TARGET = audioitemqmlplugin
TARGETPATH = AudioItem

INCLUDEPATH += $$PWD

HEADERS += \
    audioitem.h \
    mpgdecoder.h

SOURCES += \
    audioitem.cpp \
    mpgdecoder.cpp

CONFIG += link_pkgconfig
PKGCONFIG += libmpg123 alsa

load(qml_plugin)
