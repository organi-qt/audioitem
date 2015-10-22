# QT Modules
QT += qml quick

# Target
TARGET = audioitem
TEMPLATE = app

CONFIG(release, debug|release) {
#    DEFINES += QT_NO_DEBUG_OUTPUT
}

# Include Path
INCLUDEPATH += src
HEADERS += \
    src/audioitem.h

SOURCES += \
    src/main.cpp \
    src/audioitem.cpp

# Objects Pathes
DESTDIR = bin
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
OBJECTS_DIR = build
