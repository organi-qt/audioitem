TARGET = audioitemqmlplugin
TARGETPATH = AudioItem

QT += qml quick

HEADERS += \
    audioitem.h \
    mpgdecoder.h \
    audioitemqmlplugin.h

SOURCES += \
    audioitem.cpp \
    mpgdecoder.cpp \
    audioitemqmlplugin.cpp

CONFIG += link_pkgconfig
PKGCONFIG += libmpg123 alsa

load(qml_plugin)
